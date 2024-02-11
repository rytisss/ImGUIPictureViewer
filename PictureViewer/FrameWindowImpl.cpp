//This code is create by Rytis Augustauskas and it is distributed in https://github.com/rytisss/OpenGL3_ImGui_texture

#include "FrameWindowImpl.h"
#include <iostream>
#include <algorithm>
#include <exception>

#define DEFAULTINFOTEXTCOLOR_R 0.64f
#define DEFAULTINFOTEXTCOLOR_G 0.46f
#define DEFAULTINFOTEXTCOLOR_B 0.71f
#define DEFAULTINFOTEXTCOLOR_A 1.f

#define DEFAULTCANVASCOLOR_R 0.1f
#define DEFAULTCANVASCOLOR_G 0.1f
#define DEFAULTCANVASCOLOR_B 0.1f
#define DEFAULTCANVASCOLOR_A 1.f

#define MAGICGLOWNUMBER 159.15494f

#define MINWINDOWSIZE 300.f

#define MAXGRAPHICALOBJECTS 100000

FrameWindow::FrameWindowImpl::FrameWindowImpl(FrameWindow &parent, std::string name)
    : m_parent(parent),
      m_title(name),
      m_initialized(false), m_firstTime(true), m_latestImageWidth(1920),
      m_latestImageHeight(1080), m_latestResizedImageWidth(1920), m_latestResizedImageHeight(1080),
      m_checkGeometriesChange(false),
      m_windowSetDuration(0.f), m_lastFrameCaptureTime(-1), m_frameBuffer(-1),
      m_windowID(-1), m_panX(0.f), m_panY(0.f), m_zoom(1.1f), m_zoom_factor(0.05f), m_pan_factor(0.01f),
      m_mouseDownX(0.0f), m_mouseDownY(0.0f), m_mouseDownCtrlX(0.f), m_mouseDownCtrlY(0.f), m_panOnMouseDownX(0.0f),
      m_panOnMouseDownY(0.0f), m_canvasFocused(false), m_wasMouseClickedInsideCanvas(false),
      m_canvasColor(DEFAULTCANVASCOLOR_R, DEFAULTCANVASCOLOR_G, DEFAULTCANVASCOLOR_B, DEFAULTCANVASCOLOR_A),
      m_infoTextColor(DEFAULTINFOTEXTCOLOR_R, DEFAULTINFOTEXTCOLOR_G, DEFAULTINFOTEXTCOLOR_B,
                      DEFAULTINFOTEXTCOLOR_A),
      m_canvasXOffset(0), m_mouseXInCanvas(0.f), m_mouseYInCanvas(0.f), m_mouseXInTexture(0.f),
      m_mouseYInTexture(0.f), m_geometriesIDCount(-1), m_internalElementAddEnabled(true), m_latestPixelRatio(1.f),
      m_showWindowID(false), m_resizeRatio(1.f), m_resizeRatioApplied(1.f)
{
    m_latestAspectRatio = float(m_latestImageWidth) / float(m_latestImageHeight);
    ResetCanvasParameters();
    // create empty graphical object map with nullptr
    for (int i = 0; i < MAXGRAPHICALOBJECTS; i++)
    {
        m_graphicalObjectsMap[i] = nullptr;
    }
}

void FrameWindow::FrameWindowImpl::Init(int id)
{
    if (!m_initialized)
    {
        // Get the maximum texture size
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_maxTextureSize);
        std::cout << "Max texture size: " << m_maxTextureSize << std::endl;

        m_windowID = id;
        glGenTextures(1, &m_frameBuffer);
        // Define the viewport dimensions
        glBindTexture(GL_TEXTURE_2D, m_frameBuffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = {m_canvasColor.x, m_canvasColor.y, m_canvasColor.z, 1.0f};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        m_initialized = true;
        UploadInitialImage();
        m_frame.captureTime = 1;
    }
}

void FrameWindow::FrameWindowImpl::Render()
{
    std::unique_lock<std::mutex> lock(m_lock);
    //try to initialize, do nothing
    if (!m_initialized)
    { return; }
    if (ImGui::GetIO().DisplaySize.y > 0)
    {
        ImGui::PushStyleColor(ImGuiCol_ResizeGrip, 0);
        std::string window_name = m_title;
        if (m_showWindowID)
        {
            window_name += ", ID" + std::to_string(m_windowID);
        }
        static ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse;
        ImGui::SetNextWindowSizeConstraints(ImVec2(MINWINDOWSIZE, MINWINDOWSIZE),
                                            ImGui::GetIO().DisplaySize);
        if (ImGui::Begin(window_name.c_str(), nullptr, windowFlags))
        {
            if (m_firstTime)
            {
                m_firstTime = false;
                AdjustWindowSize();
                ImGui::End();
                ImGui::PopStyleColor();
                return;
            }
            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
            m_windowPos = ImGui::GetWindowPos();
            m_windowSize = ImGui::GetWindowSize();

            UpdateTexture();

            if (m_firstTime)
            {
                ImGui::End();
                ImGui::PopStyleColor();
                m_firstTime = false;
                return;
            }

            //calculate canvas position in window
            SetCanvasPosition();

            GLsizei left = 0;
            GLsizei top = 0;
            GLsizei right = static_cast<GLsizei>(m_canvasWidth);
            GLsizei bottom = static_cast<GLsizei>(m_canvasHeight);

            //calculate pixel ratio
            //it should be equal along width and height
            if (m_latestImageWidth > 0)
            {
                m_latestPixelRatio = m_canvasWidth > 0 ? m_canvasWidth / (float) m_latestImageWidth / m_zoom : 1.f;
            }
            else
            {
                m_latestPixelRatio = 1.f;
            }

            //find aspect ratio
            int viewportWidth = right;
            int viewportHeight = bottom;
            if (viewportWidth <= 0)
            { viewportWidth = 1; }
            if (viewportHeight <= 0)
            { viewportHeight = 1; }

            float viewportAspectRatio = float(viewportWidth) / float(viewportHeight);

            //https://stackoverflow.com/questions/35810782/opengl-view-projections-and-orthographic-aspect-ratio
            float widthReduction = 1.f;
            float heightReduction = 1.f;
            if (viewportAspectRatio >= m_latestAspectRatio)
            {
                widthReduction = viewportAspectRatio / m_latestAspectRatio;
            }
            else
            {
                heightReduction = m_latestAspectRatio / viewportAspectRatio;
            }

            glViewport(left, top, right, bottom);
            glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            m_textureWidthOffset = (widthReduction - 1.f) / 2.f;
            m_textureHeightOffset = (heightReduction - 1.f) / 2.f;

            //make converter
            //form canvas to texture coordinates converter
            m_canvas2TextureConverter = std::bind(&FrameWindow::FrameWindowImpl::Texture2CanvasCoord,
                                                  this,
                                                  std::placeholders::_1,
                                                  std::placeholders::_2,
                                                  std::placeholders::_3,
                                                  std::placeholders::_4);

            //check if window is active
            HandleMouseManipulations();

            //calculate texture in canvas position
            CalculateQuad();

            //calculate few internal parameters
            UpdateConverter();

            CalculateMouseCoordinateOnTexture();

            //add only if texture fits to the screen
            if (ImGui::GetIO().DisplaySize.x >= m_canvasTopLeft.x)
            {
                ImGui::PushID((void *) (intptr_t) m_frameBuffer);
                ImGui::GetWindowDrawList()->AddImageQuad(
                    (void*)(intptr_t) (m_frameBuffer),
                    m_canvasTopLeft,
                    m_canvasTopRight,
                    m_canvasBottomRight,
                    m_canvasBottomLeft,
                    m_uv_0,
                    m_uv_1,
                    m_uv_2,
                    m_uv_3,
                    IM_COL32_WHITE);
                ImGui::PopID();

                DrawGraphicalElements();
            }
            DrawFrameOnFocus();

            std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            m_windowSetDuration = (float) duration / 1000.f;
        }
        ImVec2 contextSize = ImGui::GetContentRegionAvail();
        //m_parent.m_contextWidth = (int) contextSize.x;
        //m_parent.m_contextHeight = (int) contextSize.y;
        ImGui::End();
        ImGui::PopStyleColor();
    }

    //copy graphical object if we want to inspect
    if (m_checkGeometriesChange)
    {
        std::vector<int> changeGeometriesIndices = GetChangedGraphicalObjectIndices();
        if (changeGeometriesIndices.empty())
        {
            m_parent.OnGeometryObjectChange(changeGeometriesIndices);
        }
        CopyCurrentGeometriesToOld();
    }
}

void FrameWindow::FrameWindowImpl::DeInit()
{
    if (m_initialized)
    {
        glDeleteTextures(1, &m_frameBuffer);
        // Properly deallocate all resources once they've outlived their purpose
        glDeleteFramebuffers((intptr_t) m_frameBuffer, &m_frameBuffer);
        m_initialized = false;
    }
    for (size_t i = 0; i < m_graphicalObjects.size(); i++)
    {
        UnregisterGraphicalObject(m_graphicalObjects[i]);
        delete m_graphicalObjects[i];
    }
    m_graphicalObjects.clear();
}

void FrameWindow::FrameWindowImpl::AddGraphicalObject(GraphicalObject *pGraphicalObject)
{
    std::unique_lock<std::mutex> lock(m_lock);
    if (pGraphicalObject == nullptr)
    {
        return;
    }
    //Graphical object should be initialized before adding
    if (!pGraphicalObject->IsInitialized())
    {
        unsigned int id = RegisterGraphicalObject(pGraphicalObject);
        pGraphicalObject->Init((int)id);
        m_graphicalObjects.push_back(pGraphicalObject);
    }
}

void FrameWindow::FrameWindowImpl::AddGraphicalObjects(std::vector<GraphicalObject *> &graphicalObjects)
{
    std::unique_lock<std::mutex> lock(m_lock);
    for (size_t i = 0; i < graphicalObjects.size(); i++)
    {
        if (graphicalObjects[i] == nullptr)
        {
            continue;
        }
        //Graphical object should be initialized before adding
        if (!graphicalObjects[i]->IsInitialized())
        {
            unsigned int id = RegisterGraphicalObject(graphicalObjects[i]);
            graphicalObjects[i]->Init(id);
            m_graphicalObjects.push_back(graphicalObjects[i]);
        }
    }
}

void FrameWindow::FrameWindowImpl::ChangeGraphicalObjectsActiveness(bool active, int groupId)
{
    std::unique_lock<std::mutex> lock(m_lock);
    // change all elements color
    for (size_t i = 0; i < m_graphicalObjects.size(); i++)
    {
        if (m_graphicalObjects[i]->GetGroup() == groupId || groupId == -1)
        {
            m_graphicalObjects[i]->Active(active);
        }
    }
}

void FrameWindow::FrameWindowImpl::ChangeGraphicalObjectsVisibility(bool visibility, int groupId)
{
    std::unique_lock<std::mutex> lock(m_lock);
    // change all elements color
    for (size_t i = 0; i < m_graphicalObjects.size(); i++)
    {
        if (m_graphicalObjects[i]->GetGroup() == groupId || groupId == -1)
        {
            m_graphicalObjects[i]->Visible(visibility);
        }
    }
}

void FrameWindow::FrameWindowImpl::ChangeGraphicalObjectsColor(const Color& color, int groupId)
{
    std::unique_lock<std::mutex> lock(m_lock);
    // change all elements color
    for (size_t i = 0; i < m_graphicalObjects.size(); i++)
    {
        if (m_graphicalObjects[i]->GetGroup() == groupId || groupId == -1)
        {
            m_graphicalObjects[i]->SetColor(color);
        }
    }
}

void FrameWindow::FrameWindowImpl::ChangeGraphicalObjectsNoteVisibility(bool visibility, int groupId)
{
    std::unique_lock<std::mutex> lock(m_lock);
    // change all elements color
    for (size_t i = 0; i < m_graphicalObjects.size(); i++)
    {
        if (m_graphicalObjects[i]->GetGroup() == groupId || groupId == -1)
        {
            m_graphicalObjects[i]->NotesVisible(visibility);
        }
    }
}

void FrameWindow::FrameWindowImpl::DeleteGraphicalObject(GraphicalObject *pGraphicalObject)
{
    std::unique_lock<std::mutex> lock(m_lock);
    for (size_t i = 0; i < m_graphicalObjects.size(); i++)
    {
        if (pGraphicalObject == m_graphicalObjects[i])
        {
            m_graphicalObjects[i]->DeInit();
            UnregisterGraphicalObject(m_graphicalObjects[i]);
            delete m_graphicalObjects[i];
            m_graphicalObjects[i] = nullptr;
        }
    }
    //take out empty graphical objects
    for (size_t i = 0; i < m_graphicalObjects.size(); i++)
    {
        if (m_graphicalObjects[i] == nullptr)
        {
            m_graphicalObjects.erase(m_graphicalObjects.begin() + i);
            i--;
        }
    }
}

void FrameWindow::FrameWindowImpl::DeleteGraphicalObject(int id)
{
    std::unique_lock<std::mutex> lock(m_lock);
    for (size_t i = 0; i < m_graphicalObjects.size(); i++)
    {
        if (id == m_graphicalObjects[i]->GetID())
        {
            m_graphicalObjects[i]->DeInit();
            UnregisterGraphicalObject(m_graphicalObjects[i]);
            delete m_graphicalObjects[i];
            m_graphicalObjects[i] = nullptr;
        }
    }
    //take out empty graphical objects
    for (size_t i = 0; i < m_graphicalObjects.size(); i++)
    {
        if (m_graphicalObjects[i] == nullptr)
        {
            m_graphicalObjects.erase(m_graphicalObjects.begin() + i);
            i--;
        }
    }
}

std::vector<GraphicalObject *> FrameWindow::FrameWindowImpl::GetGraphicalObjects()
{
    return m_graphicalObjects;
}

void FrameWindow::FrameWindowImpl::SetName(std::string title)
{
    std::unique_lock<std::mutex> lock(m_lock);
    m_title = title;
}

void FrameWindow::FrameWindowImpl::SetAutoResize(bool state)
{
}

int FrameWindow::FrameWindowImpl::GetWindowWidth()
{
    return 0;
}

int FrameWindow::FrameWindowImpl::GetWindowHeight()
{
    return 0;
}

bool FrameWindow::FrameWindowImpl::IsWindowMovementDisabled()
{
    return m_canvasFocused;
}

void FrameWindow::FrameWindowImpl::EnableControlElementsAdd(bool status)
{
    m_internalElementAddEnabled = status;
}

void FrameWindow::FrameWindowImpl::EnableGeometriesChangeNotification(bool status)
{
    m_checkGeometriesChange = status;
}

bool FrameWindow::FrameWindowImpl::IsControlElementsAddEnabled()
{
    return m_internalElementAddEnabled;
}

void FrameWindow::FrameWindowImpl::ShowWindowIDInTitle(bool status)
{
    m_showWindowID = status;
}

void FrameWindow::FrameWindowImpl::GetPanZoom(float &panX, float &panY, float &zoom)
{
    panX = m_panX;
    panY = m_panY;
    zoom = m_zoom;
}

void FrameWindow::FrameWindowImpl::SetPanZoom(float panX, float panY, float zoom)
{
    m_panX = panX;
    m_panY = panY;
    m_zoom = zoom;
}

std::string FrameWindow::FrameWindowImpl::GetName()
{
    return m_title;
}

std::string FrameWindow::FrameWindowImpl::GetConfig()
{
    //make nested configuration of all graphical control elements
    nlohmann::json graphicalElementsConfig;
    int graphicalElementCounter = 0;
    for (size_t i = 0; i < m_graphicalObjects.size(); i++)
    {
        std::string graphicalElementConfig = m_graphicalObjects[i]->GetConfig();
        if (graphicalElementConfig.empty())
        {
            continue;
        }
        //append json with nested graphical control json
        std::string index = std::to_string(graphicalElementCounter);
        graphicalElementsConfig[index] = nlohmann::json::parse(graphicalElementConfig);
        graphicalElementCounter++;
    }
    return graphicalElementsConfig.dump();
}

void FrameWindow::FrameWindowImpl::SetConfig(std::string &config)
{
    //try to parse and load config
    try
    {
        nlohmann::json graphicalElementsConfigs = nlohmann::json::parse(config);
        for (auto &graphicalElementConfig : graphicalElementsConfigs.items())
        {
            // deserialize, get type and recreate object
            nlohmann::json graphicalElementConfigJson = nlohmann::json::parse(graphicalElementConfig.value().dump());
            GraphicalObject *graphicalElement = nullptr;
            std::string graphicalElementConfigStr = graphicalElementConfigJson.dump();
            if (graphicalElementConfigJson["type"] == "CircleGraphics")
            {
                graphicalElement = new CircleGraphics();
                graphicalElement->SetConfig(graphicalElementConfigStr);
            }
            else
            {
                if (graphicalElementConfigJson["type"] == "RectangleGraphics")
                {
                    graphicalElement = new RectangularGraphics();
                    graphicalElement->SetConfig(graphicalElementConfigStr);
                }
                else
                {
                    if (graphicalElementConfigJson["type"] == "PolygonGraphics")
                    {
                        graphicalElement = new PolygonGraphics();
                        graphicalElement->SetConfig(graphicalElementConfigStr);
                    }
                }
            }

            if (graphicalElement != nullptr)
            {
                this->AddGraphicalObject(graphicalElement);
                //increment 'unique id' if added object has bigger id
                if (graphicalElement->GetID() > m_geometriesIDCount)
                {
                    m_geometriesIDCount = graphicalElement->GetID();
                }
            }
        }
    }
    catch (...)
    {
        std::cout << "Adding element went wrong..." << std::endl;
    }
}

void FrameWindow::FrameWindowImpl::UpdateFrame(const Frame &frame)
{
    std::unique_lock<std::mutex> lock(m_lock);
    //update only if captureTime/timeStamp is different
    if (frame.Empty())
    {
        return;
    }
    m_frame = frame;
    m_frame.index = frame.index;
    m_frame.captureTime = std::chrono::duration_cast<std::chrono::milliseconds>(                                                                                  
        std::chrono::system_clock::now().time_since_epoch()).count();//frame.captureTime;

    // just override to check, TODO: delete
    m_maxTextureSize = 2048;
    if (m_frame.width > m_maxTextureSize || m_frame.height > m_maxTextureSize)
    {
        float widthResize = std::ceil((float)m_frame.width / (float)m_maxTextureSize);
        float heightResize = std::ceil((float)m_frame.height / (float)m_maxTextureSize);
        float downsizeRatio = widthResize > heightResize ? widthResize : heightResize;
        DownscaleFrame(m_frame, m_fitFrame, int(downsizeRatio));
        m_resizeRatio = 1.f / downsizeRatio;
    }
    else
    {
        m_resizeRatio = 1.f;
        m_fitFrame = m_frame;
    }

    if (m_resizeRatio <= 0.f)
    {
        //can happen while loading gui
        m_resizeRatio = 1.f;
        m_frame = frame;
        //Image2RGBA(frame.image, m_frame.image);
    }
    else
    {
        //TODO
        //cv::Mat temp;
        //cv::resize(frame.image, temp, cv::Size(0, 0), m_resizeRatio, m_resizeRatio);
        //Image2RGBA(temp, m_frame.image);
        //temp.release();
    }
}

void FrameWindow::FrameWindowImpl::AdjustWindowSize()
{
    ImVec2 winPos = ImGui::GetWindowPos();
    ImVec2 winSize = ImGui::GetWindowSize();
    ImVec2 viewportSize = ImGui::GetIO().DisplaySize;

    float adjust_w = winSize.x > viewportSize.x ? viewportSize.x : winSize.x;
    float adjust_h = winSize.y > viewportSize.y ? viewportSize.y : winSize.y;

    float adjust_x = 0.f;
    float adjust_y = 0.f;
    if (winPos.x < 0.f)
    {
        adjust_x = 0.f;
    }
    else
    {
        if (winPos.x + winSize.x > viewportSize.x)
        {
            adjust_x = viewportSize.x - adjust_w;
        }
        else
        {
            adjust_x = winPos.x;
        }
    }

    if (winPos.y < 0.f)
    {
        adjust_y = 0.f;
    }
    else
    {
        if (winPos.y + winSize.y > viewportSize.y)
        {
            adjust_y = viewportSize.y - adjust_h;
        }
        else
        {
            adjust_y = winPos.y;
        }
    }

    ImGui::SetWindowPos(ImVec2(adjust_x, adjust_y));
    ImGui::SetWindowSize(ImVec2(adjust_w, adjust_h));
}

void FrameWindow::FrameWindowImpl::UpdateTexture()
{
    if (m_lastFrameCaptureTime == m_frame.captureTime)
    {
        return;
    }
    int width = m_fitFrame.width;
    int height = m_fitFrame.height;
    if (!m_fitFrame.Empty())
    {
        // take a part of image matrix
        glBindTexture(GL_TEXTURE_2D, m_frameBuffer);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGBA,
                     width,
                     height,
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     m_fitFrame.data.get());
        glGenerateMipmap(GL_TEXTURE_2D);
        //glCopyImageSubData
    }

    if (m_latestResizedImageWidth != width || m_latestResizedImageHeight != height)
    {
        m_parent.ImageSizeChange(m_latestImageWidth, m_latestImageHeight, width, height);
    }

    m_latestImageWidth = width / m_resizeRatio; //reset to original
    m_latestImageHeight = height / m_resizeRatio; //reset to original
    if (m_latestImageWidth <= 0)
    { m_latestImageWidth = 1; }
    if (m_latestImageHeight <= 0)
    { m_latestImageHeight = 1; }
    m_latestAspectRatio = float(m_latestImageWidth) / float(m_latestImageHeight);
    m_lastFrameCaptureTime = m_fitFrame.captureTime;
    m_latestResizedImageWidth = width;
    m_latestResizedImageHeight = height;
    if (m_latestResizedImageWidth <= 0)
    { m_latestResizedImageWidth = 1; }
    if (m_latestResizedImageHeight <= 0)
    { m_latestResizedImageHeight = 1; }
}

void FrameWindow::FrameWindowImpl::HandlePan(float dx, float dy)
{
    m_panX += dx;
    m_panY += dy;
}

void FrameWindow::FrameWindowImpl::SetPan(float panX, float panY)
{
    m_panX = panX;
    m_panY = panY;
}

void FrameWindow::FrameWindowImpl::HandleZoom(float factor)
{
    if (m_zoom + factor > 0.01f) //prevent 'overzooming'
    {
        m_zoom += factor;
    }
}

void FrameWindow::FrameWindowImpl::HandlePopUp()
{
    ImGui::OpenPopup("Control Graphics");
    GraphicalObject *pGraphicsObject = nullptr;
    if (ImGui::BeginPopup("Control Graphics"))
    {
        ImGui::Text("Control Elements:");
        ImGui::Separator();
//        for (size_t i = 0; i < (size_t) Control::Graphics::NumberOfGraphics; i++)
//        {
//            Control::Graphics graphicsObject = (Control::Graphics) i;
//            std::string graphicsName = Control::GetName(graphicsObject);
//            if (ImGui::Selectable(graphicsName.c_str()))
//            {
//                pGraphicsObject = Control::CreateGraphicalObject(graphicsObject);
//                if (pGraphicsObject != nullptr)
//                {
//                    pGraphicsObject->Init(this->GetNextGraphicalObjectID());
//                    m_graphicalObjects.push_back(pGraphicsObject);
//                }
//                else
//                {
//                }
//            }
//        }
        ImGui::EndPopup();
    }
}

float FrameWindow::FrameWindowImpl::RenderInputParameterLine(float currentValue, std::string name, bool round)
{
    std::string value_text = std::to_string(currentValue);
    static char value_text_char[32];
    std::strcpy(value_text_char, value_text.c_str());
    ImGui::InputText(name.c_str(), value_text_char, IM_ARRAYSIZE(value_text_char), ImGuiInputTextFlags_CharsDecimal);
    double inputValue = std::atof(value_text_char);
    if (round)
    {
        inputValue = std::round(inputValue);
    }
    return (float) inputValue;
}

void FrameWindow::FrameWindowImpl::RenderObjectNote(const std::map<std::string, std::string>& note)
{
    if (note.empty())
    {
        return;
    }
    ImGui::BeginTooltip();
    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 25.0f);
    // make description of each map element in different line
    std::string noteText;
    for (auto const& entry : note)
    {
        noteText += entry.first + ": " + entry.second;
        noteText += "\n";
    }
    ImGui::TextUnformatted(noteText.c_str());
    ImGui::PopTextWrapPos();
    ImGui::EndTooltip();
}

void FrameWindow::FrameWindowImpl::SetCanvasPosition()
{
    float leftOffset = 5.f;
#ifdef RESETCOLORBUTTONS
    leftOffset += m_canvasXOffset;
#endif
#ifdef RESETCOLORBUTTONS
    float topOffset = 55.f; // for two line 55, for three line at least 85
#else
    float topOffset = 25.f; // for two line 55, for three line at least 85
#endif
    float rightOffset = 5.f;
    float bottomOffset = 5.f;

    m_canvasTopLeft.x = m_windowPos.x + leftOffset;
    m_canvasTopLeft.y = m_windowPos.y + topOffset;
    m_canvasBottomRight.x = m_windowPos.x + m_windowSize.x - rightOffset;
    m_canvasBottomRight.y = m_canvasTopRight.y + m_windowSize.y - topOffset - bottomOffset;
    m_canvasTopRight.x = m_canvasBottomRight.x;
    m_canvasTopRight.y = m_canvasTopLeft.y;
    m_canvasBottomLeft.x = m_canvasTopLeft.x;
    m_canvasBottomLeft.y = m_canvasBottomRight.y;

    m_canvasWidth = m_canvasTopRight.x - m_canvasTopLeft.x;
    m_canvasHeight = m_canvasBottomLeft.y - m_canvasTopLeft.y;
}

void FrameWindow::FrameWindowImpl::DrawContextWindow()
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
    //ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::BeginChild("Coordinates", ImVec2(m_canvasXOffset * 0.9f, ImGui::GetContentRegionAvail().y), true,
                      window_flags);
    if (ImGui::CollapsingHeader("Canvas", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::TreeNodeEx("Image", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //remove spacing
            ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
            std::string imageWidth = "W: " + std::to_string((int) m_imageWidth) + " px";
            ImGui::TextColored(m_infoTextColor, "%s", imageWidth.c_str());
            std::string imageHeight = "H: " + std::to_string((int) m_imageHeight) + " px";
            ImGui::TextColored(m_infoTextColor, "%s", imageHeight.c_str());
            //add spacing
            ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
            ImGui::TreePop();
        }
        if (ImGui::TreeNodeEx("Mouse", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //remove spacing
            ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Canvas");
            std::string canvasX = "X: " + std::to_string(m_mouseXInCanvas);
            ImGui::TextColored(m_infoTextColor, "%s", canvasX.c_str());
            std::string canvasY = "Y: " + std::to_string(m_mouseYInCanvas);
            ImGui::TextColored(m_infoTextColor, "%s", canvasY.c_str());
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Texture");
            std::string textureX = "X: " + std::to_string(m_mouseXInTexture);
            ImGui::TextColored(m_infoTextColor, "%s", textureX.c_str());
            std::string textureY = "Y: " + std::to_string(m_mouseYInTexture);
            ImGui::TextColored(m_infoTextColor, "%s", textureY.c_str());
            //add spacing
            ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
            ImGui::TreePop();
        }
        if (ImGui::TreeNodeEx("Control", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //remove spacing
            ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

            bool zoom_p = ImGui::Button("zoom+");
            ImGui::SameLine();
            bool zoom_m = ImGui::Button("zoom-");
            bool pan_left = ImGui::ArrowButton("##left", ImGuiDir_Left);
            ImGui::SameLine();
            bool pan_right = ImGui::ArrowButton("##right", ImGuiDir_Right);
            ImGui::SameLine();
            bool pan_down = ImGui::ArrowButton("##down", ImGuiDir_Down);
            ImGui::SameLine();
            bool pan_up = ImGui::ArrowButton("##up", ImGuiDir_Up);
            bool reset_coord = ImGui::Button("Reset");
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Texture Pos");
            std::string panXText = "X: " + std::to_string(m_panX);
            ImGui::TextColored(m_infoTextColor, "%s", panXText.c_str());
            std::string panYText = "Y: " + std::to_string(m_panY);
            ImGui::TextColored(m_infoTextColor, "%s", panYText.c_str());
            std::string zoomText = "Z: " + std::to_string(m_zoom);
            ImGui::TextColored(m_infoTextColor, "%s", zoomText.c_str());
            if (zoom_p)
            { HandleZoom(-m_zoom_factor); }
            if (zoom_m)
            { HandleZoom(m_zoom_factor); }
            if (pan_up)
            { HandlePan(0.f, m_pan_factor); }
            if (pan_down)
            { HandlePan(0.f, -m_pan_factor); }
            if (pan_left)
            { HandlePan(-m_pan_factor, 0.f); }
            if (pan_right)
            { HandlePan(m_pan_factor, 0.f); }
            if (reset_coord)
            {
                m_zoom = 1.f;
                m_panX = m_panY = 0.f;
            }
            //add spacing
            ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
            ImGui::TreePop();
        }

    }
    if (ImGui::CollapsingHeader("Graphics", ImGuiTreeNodeFlags_DefaultOpen))
    {
        //Graphical elements information render
        RenderGraphicalObjectsParameters();
    }
    ImGui::EndChild();
    //ImGui::PopStyleVar();
}

void FrameWindow::FrameWindowImpl::RenderGraphicalObjectsParameters()
{

}

void FrameWindow::FrameWindowImpl::CalculateQuad()
{
    //center of image
    float zoomingPtX = 0.5f;
    float zoomingPtY = 0.5f;

    m_uv_0.x = zoomingPtX + (0.0f - m_textureWidthOffset - zoomingPtX) * m_zoom + m_panX;
    m_uv_0.y = zoomingPtY + (0.0f - m_textureHeightOffset - zoomingPtY) * m_zoom + m_panY; //top left
    m_uv_1.x = zoomingPtX + (1.0f + m_textureWidthOffset - zoomingPtX) * m_zoom + m_panX;
    m_uv_1.y = zoomingPtY + (0.0f - m_textureHeightOffset - zoomingPtY) * m_zoom + m_panY; //top right
    m_uv_2.x = zoomingPtX + (1.0f + m_textureWidthOffset - zoomingPtX) * m_zoom + m_panX;
    m_uv_2.y = zoomingPtY + (1.0f + m_textureHeightOffset - zoomingPtY) * m_zoom + m_panY; //bottom right
    m_uv_3.x = zoomingPtX + (0.0f - m_textureWidthOffset - zoomingPtX) * m_zoom + m_panX;
    m_uv_3.y = zoomingPtY + (1.0f + m_textureHeightOffset - zoomingPtY) * m_zoom + m_panY; //bottom left
}

void FrameWindow::FrameWindowImpl::HandleMouseManipulations()
{
    SortGraphicalObjectByUpdateTime();

    ImGuiIO &io = ImGui::GetIO();
    float mouseX = io.MousePos.x;
    float mouseY = io.MousePos.y;

    bool cursorOnCanvas =
        (m_canvasTopLeft.x <= mouseX) && (m_canvasTopRight.x > mouseX) && (m_canvasTopLeft.y <= mouseY) &&
            (m_canvasBottomLeft.y > mouseY);

    bool windowHovered = ImGui::IsWindowHovered();
    bool windowFocused = ImGui::IsWindowFocused();
    //update status of focused canvas
    m_canvasFocused = cursorOnCanvas && windowHovered;

    /* DEPRECATED CAUSE MOVEMENT STATUS WILL BE UPDATED FROM GLOBAL FIELD WITH DECISION FROM EVERY FRAME WINDOW
    //enable/disable window drag with left mouse button down
    io.ConfigWindowsMoveFromTitleBarOnly = m_canvasFocused;
    */

    bool mouseLeftReleased = ImGui::IsMouseReleased(0);
    bool mouseRightReleased = ImGui::IsMouseReleased(1);
    if (mouseRightReleased)
    {
        m_wasMouseClickedInsideCanvas = false;
    }
    //First get object that mouse in on and then set an action of release
    if (mouseLeftReleased)
    {
        for (size_t i = 0; i < m_graphicalObjects.size(); i++)
        {
            m_graphicalObjects[i]->MouseLeftState(false);
        }
    }
    if (mouseRightReleased)
    {
        for (size_t i = 0; i < m_graphicalObjects.size(); i++)
        {
            m_graphicalObjects[i]->MouseRightState(false);
        }
    }

    if (m_canvasFocused) // check also if active
    {
        if (io.MouseWheel > 0.f)
        {
            HandleZoom(-m_zoom_factor);
        }
        else
        {
            if (io.MouseWheel < 0.f)
            {
                HandleZoom(m_zoom_factor);
            }
        }
        //save last pan and mouse coordinates if right mouse button is clicked
        if (ImGui::IsMouseClicked(1))
        {
            m_mouseDownX = mouseX;
            m_mouseDownY = mouseY;
            m_panOnMouseDownX = m_panX;
            m_panOnMouseDownY = m_panY;
            m_wasMouseClickedInsideCanvas = true;
        }
        if (ImGui::IsMouseDown(1) && m_wasMouseClickedInsideCanvas)
        {
            float width_res = m_canvasTopRight.x - m_canvasBottomLeft.x;
            if (width_res <= 0.f)
            {
                width_res = 1.f;
            }
            float height_res = m_canvasBottomLeft.y - m_canvasTopLeft.y;
            if (height_res <= 0.f)
            {
                height_res = 1.f;
            }
            float deltaX_ = m_panOnMouseDownX +
                (m_mouseDownX - mouseX) / width_res * (1.f + m_textureWidthOffset * 2.f) * m_zoom;
            float deltaY_ = m_panOnMouseDownY +
                (m_mouseDownY - mouseY) / height_res * (1.f + m_textureHeightOffset * 2.f) * m_zoom;
            SetPan(deltaX_, deltaY_);
        }
        if (ImGui::IsMouseClicked(0) && ImGui::GetIO().KeyCtrl)
        {
            m_mouseDownCtrlX = mouseX;
            m_mouseDownCtrlY = mouseY;
        }
        if (ImGui::GetIO().KeyCtrl) //make focused object to indicate more significantly
        {
            for (size_t i = 0; i < m_graphicalObjects.size(); i++)
            {
                if (m_graphicalObjects[i]->IsActive() && m_graphicalObjects[i]->IsControlled())
                {
                    m_graphicalObjects[i]->SetFocus(true);
                }
            }
        }
        if (ImGui::IsMouseDown(0) && ImGui::GetIO().KeyCtrl)
        {
            //offset all region
            float deltaX_ = -(m_mouseDownCtrlX - mouseX) / m_latestPixelRatio;
            float deltaY_ = -(m_mouseDownCtrlY - mouseY) / m_latestPixelRatio;
            for (size_t i = 0; i < m_graphicalObjects.size(); i++)
            {
                if (m_graphicalObjects[i]->IsActive() && m_graphicalObjects[i]->IsControlled())
                {
                    m_graphicalObjects[i]->Offset(deltaX_, deltaY_);
                }
            }
            //save latest position and offset next time from this point
            m_mouseDownCtrlX = mouseX;
            m_mouseDownCtrlY = mouseY;
            return;
        }


        //Interested only what happened inside canvas region
        //Register again all the clicks that happened to the graphical objects inside window

        //First check released buttons and do action according

        //Control objects is mouse is down
        bool mouseLeftDown = ImGui::IsMouseDown(0);
        bool mouseRightDown = ImGui::IsMouseDown(1);
        //First check which object is controlled and if nothing is, check which is focused
        GraphicalObject *controlledObject = nullptr;
        for (size_t i = 0; i < m_graphicalObjects.size(); i++)
        {
            if (m_graphicalObjects[i]->IsControlled() && m_graphicalObjects[i]->IsActive())
            {
                controlledObject = m_graphicalObjects[i];
                break;//no need to search for more, because object was found
            }
        }
        if (controlledObject != nullptr && mouseLeftDown)
        {
            if (controlledObject->IsActive())
            {
                controlledObject->OnLeftDown(m_mouseXInTexture, m_mouseYInTexture, m_canvas2TextureConverter,
                                             m_canvasTopLeft[0], m_canvasTopLeft[1], m_latestPixelRatio, m_resizeRatio);
            }
        }
        else
        {
            if (controlledObject != nullptr && mouseRightDown)
            {
                if (controlledObject->IsActive())
                {
                    controlledObject->OnRightDown(m_mouseXInTexture, m_mouseYInTexture, m_canvas2TextureConverter,
                                                  m_canvasTopLeft[0], m_canvasTopLeft[1], m_latestPixelRatio,
                                                  m_resizeRatio);
                }
            }
        }
        //if something is found, no need to continue doing further
        if (controlledObject != nullptr)
        {
            return;
        }

        //Register mouse click
        //Get control element that is in on the mouse
        GraphicalObject *inRangeObject = nullptr;
        for (size_t i = 0; i < m_graphicalObjects.size(); i++)
        {
            // should it be handle like this??? TODO
            if (!m_graphicalObjects[i]->IsActive())
            {
                continue;
            }
            if (m_graphicalObjects[i]->IsMouseOnObject(m_mouseXInTexture, m_mouseYInTexture, m_canvas2TextureConverter,
                                                       m_canvasTopLeft[0], m_canvasTopLeft[1], m_latestPixelRatio,
                                                       m_resizeRatio))
            {
                inRangeObject = m_graphicalObjects[i];
                break;//no need to search for more, because object was found
            }
        }
        //If object is found, register click or focused
        //Priority will go as following mouse_click_left->mouse_click_right->hovering_over
        bool mouseLeftClicked = ImGui::IsMouseClicked(0);
        bool mouseRightClicked = ImGui::IsMouseClicked(1);
        if (inRangeObject != nullptr)
        {
            if (mouseLeftClicked)
            {
                inRangeObject->MouseLeftState(true);
                inRangeObject->MouseLeftClickUpdate(m_mouseXInTexture, m_mouseYInTexture, m_resizeRatio);
                //turn of focus of all other objects
                for (auto &graphicalObject : m_graphicalObjects)
                {
                    if (inRangeObject->GetID() != graphicalObject->GetID())
                    {
                        graphicalObject->SetFocus(false);
                    }
                }
            }
            else
            {
                if (mouseRightClicked)
                {
                    inRangeObject->MouseRightState(true);
                    inRangeObject->MouseRightClickUpdate(m_mouseXInTexture, m_mouseYInTexture, m_resizeRatio);
                    //turn of focus of all other objects
                    for (auto &graphicalObject : m_graphicalObjects)
                    {
                        if (inRangeObject->GetID() != graphicalObject->GetID())
                        {
                            graphicalObject->MouseRightState(false);
                        }
                    }
                }
                else //hovered over
                {
                    inRangeObject->MouseHoveredOver(true);
                    inRangeObject->MouseCursorHoverUpdate(m_mouseXInTexture, m_mouseYInTexture, m_resizeRatio);
                    for (auto &graphicalObject : m_graphicalObjects)
                    {
                        if (inRangeObject->GetID() != graphicalObject->GetID())
                        {
                            graphicalObject->MouseHoveredOver(false);
                        }
                    }
                }
            }
        }
        else
        {
            for (auto &graphicalObject : m_graphicalObjects)
            {
                graphicalObject->MouseHoveredOver(false);
            }
            if (mouseLeftClicked)
            {
                for (auto &graphicalObject : m_graphicalObjects)
                {
                    graphicalObject->SetFocus(false);
                }
            }
            if (mouseRightClicked)
            {
                for (auto &graphicalObject : m_graphicalObjects)
                {
                    graphicalObject->MouseRightState(false);
                }
            }
        }
    }
}

void FrameWindow::FrameWindowImpl::DrawGraphicalElements()
{
    ImGui::GetWindowDrawList()->PushClipRect(m_canvasTopLeft, m_canvasBottomRight, true);
    for (size_t i = 0; i < m_graphicalObjects.size(); i++)
    {
        m_graphicalObjects[i]->Render(m_canvas2TextureConverter,
            m_canvasTopLeft[0],
            m_canvasTopLeft[1],
            m_latestPixelRatio,
            m_resizeRatio);
    }
    ImGui::GetWindowDrawList()->PopClipRect();
    // render notes if mouse hovered over
    for (const auto& graphicalElement : m_graphicalObjects)
    {
        if (graphicalElement->IsMouseHoveredOver())
        {
            if (graphicalElement->IsNoteVisible())
            {
                RenderObjectNote(graphicalElement->GetNote());
                break;
            }
        }
    }
}

void FrameWindow::FrameWindowImpl::CalculateMouseCoordinateOnTexture()
{
    ImGuiIO &io = ImGui::GetIO();
    m_mouseXInCanvas = io.MousePos.x - m_canvasTopLeft.x;
    m_mouseYInCanvas = io.MousePos.y - m_canvasTopLeft.y;
    Canvas2TextureCoord(m_mouseXInCanvas, m_mouseYInCanvas, m_mouseXInTexture, m_mouseYInTexture);
}

void FrameWindow::FrameWindowImpl::UpdateConverter()
{
    m_imageWidth = m_latestImageWidth == 0 ? 1920.f : m_latestImageWidth;
    m_imageHeight = m_latestImageHeight == 0 ? 1080.f : m_latestImageHeight;

    m_widthRatio = m_imageWidth / m_canvasWidth * (1.f + m_textureWidthOffset * 2.f) * m_zoom;
    m_heightRatio = m_imageHeight / m_canvasHeight * (1.f + m_textureHeightOffset * 2.f) * m_zoom;
}

void FrameWindow::FrameWindowImpl::ChangeCanvasColor(ImVec4 color)
{
    float borderColor[] = {color.x, color.y, color.z, 1.0f};
    glBindTexture(GL_TEXTURE_2D, (intptr_t) m_frameBuffer);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
}

void FrameWindow::FrameWindowImpl::Canvas2TextureCoord(float canvasX, float canvasY, float &textureX, float &textureY)
{
    textureX = canvasX * m_widthRatio + m_imageWidth * m_uv_0.x;
    textureY = canvasY * m_heightRatio + m_imageHeight * m_uv_0.y;
}

void FrameWindow::FrameWindowImpl::Texture2CanvasCoord(float textureX, float textureY, float &canvasX, float &canvasY)
{
    canvasX = (textureX - m_imageWidth * m_uv_0.x) / m_widthRatio;
    canvasY = (textureY - m_imageHeight * m_uv_0.y) / m_heightRatio;
}

void FrameWindow::FrameWindowImpl::DrawFrameOnFocus()
{
    int timestamp = (int)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() % 1000; //miliseconds only matter
    float thickness = 1.f;
    ImU32 color = IM_COL32(0, 0, 0, 255);
    if (m_canvasFocused)
    {
        std::uint8_t firstGlowValue = (std::uint8_t) (std::sin((float) timestamp / MAGICGLOWNUMBER) * 100.f) +
            100.f; //make value between 150 and 250
        std::uint8_t secondGlowValue = (std::uint8_t) (std::cos((float) timestamp / MAGICGLOWNUMBER) * 100.f) +
            150.f; //make value between 150 and 250
        //render differently if control key on keyboard is pressed
        if (ImGui::GetIO().KeyCtrl)
        {
            color = IM_COL32(secondGlowValue, 150, firstGlowValue, 255);
            thickness = 5.f;
        }
        else
        {
            color = IM_COL32(secondGlowValue, 100, firstGlowValue, 150);
            thickness = 2.f;
        }
    }
    ImGui::GetWindowDrawList()->AddRect(m_canvasTopLeft,
                                        m_canvasBottomRight,
                                        color,
                                        0.f,
                                        0,
                                        thickness);
}

void FrameWindow::FrameWindowImpl::SortGraphicalObjectByUpdateTime()
{
    for (size_t i = 0; i < m_graphicalObjects.size(); i++)
    {
        for (size_t j = i + 1; j < m_graphicalObjects.size(); j++)
        {
            if (m_graphicalObjects[i]->GetLastUpdateTime() < m_graphicalObjects[j]->GetLastUpdateTime())
            {
                GraphicalObject *temp = m_graphicalObjects[j];
                m_graphicalObjects[j] = m_graphicalObjects[i];
                m_graphicalObjects[i] = temp;
            }
        }
    }
}

unsigned int FrameWindow::FrameWindowImpl::GetNextGraphicalObjectID()
{
    int highestID = 0;
    for (size_t i = 0; i < m_graphicalObjects.size(); i++)
    {
        int currentObjectID = m_graphicalObjects[i]->GetID();
        if (currentObjectID > highestID)
        {
            highestID = currentObjectID;
        }
    }
    //increment id to be next
    highestID++;
    return (unsigned int) highestID;
}

void FrameWindow::FrameWindowImpl::AdjustGraphicalObjectsIDs(int deletedID)
{
    for (size_t i = 0; i < m_graphicalObjects.size(); i++)
    {
        int currentObjectID = m_graphicalObjects[i]->GetID();
        if (currentObjectID > deletedID)
        {
            m_graphicalObjects[i]->SetID(currentObjectID++);
        }
    }
}

void FrameWindow::FrameWindowImpl::UploadInitialImage()
{
    std::uint32_t width = 640;
    std::uint32_t height = 480;
    std::uint8_t channels = 4;
    std::uint8_t* data = new std::uint8_t[width * height * (std::uint32_t)channels]();
    m_frame = Frame(width, height, channels, data);
    m_latestImageWidth = (int)width;
    m_latestImageHeight = (int)height;
}

void FrameWindow::FrameWindowImpl::ResetCanvasParameters()
{
    m_canvasTopLeft.x = 0.f;
    m_canvasTopLeft.y = 0.f;
    m_canvasTopRight.x = 0.f;
    m_canvasTopRight.y = 0.f;
    m_canvasBottomRight.x = 0.f;
    m_canvasBottomRight.y = 0.f;
    m_canvasBottomLeft.x = 0.f;
    m_canvasBottomLeft.y = 0.f;
    m_uv_0.x = 0.f;
    m_uv_0.y = 0.f;
    m_uv_1.x = 1.f;
    m_uv_1.y = 0.f;
    m_uv_2.x = 1.f;
    m_uv_2.y = 1.f;
    m_uv_3.x = 0.f;
    m_uv_3.y = 1.f;
    m_textureWidthOffset = 0.f;
    m_textureHeightOffset = 0.f;
    m_widthRatio = 1.f;
    m_heightRatio = 1.f;
    m_canvasWidth = m_canvasTopRight.x - m_canvasTopLeft.x;
    m_canvasHeight = m_canvasBottomLeft.y - m_canvasTopLeft.y;
    m_imageWidth = 1920.f;
    m_imageHeight = 1080.f;
}

void FrameWindow::FrameWindowImpl::CopyCurrentGeometriesToOld()
{
    //clear vector before copying
    for (size_t i = 0; i < m_oldGraphicalElements.size(); i++)
    {
        delete m_oldGraphicalElements[i];
    }
    m_oldGraphicalElements.clear();

    for (size_t i = 0; i < m_graphicalObjects.size(); i++)
    {
        //check what derived class object is
        RectangularGraphics *pRect = dynamic_cast<RectangularGraphics *>(m_graphicalObjects[i]);
        if (pRect != nullptr)
        {
            GraphicalObject *pRectCopy = new RectangularGraphics(*pRect);
            m_oldGraphicalElements.push_back(pRectCopy);
            continue;
        }
        PolygonGraphics *pPoly = dynamic_cast<PolygonGraphics *>(m_graphicalObjects[i]);
        if (pPoly != nullptr)
        {
            GraphicalObject *pPolyCopy = new PolygonGraphics(*pPoly);
            m_oldGraphicalElements.push_back(pPolyCopy);
            continue;
        }
        CircleGraphics *pCirc = dynamic_cast<CircleGraphics *>(m_graphicalObjects[i]);
        if (pCirc != nullptr)
        {
            GraphicalObject *pCircCopy = new CircleGraphics(*pCirc);
            m_oldGraphicalElements.push_back(pCircCopy);
            continue;
        }
    }
}

std::vector<int> FrameWindow::FrameWindowImpl::GetChangedGraphicalObjectIndices()
{
    std::vector<int> changeGeometriesIndices;
    //if the size of old and new vector are different, generate event with additional index -1
    if (m_oldGraphicalElements.size() != m_graphicalObjects.size())
    {
        changeGeometriesIndices.push_back(-1);
    }
    for (size_t i = 0; i < m_graphicalObjects.size(); i++)
    {
        int currentObjectIndex = m_graphicalObjects[i]->GetID();
        bool objectFound = false;
        for (size_t j = 0; j < m_oldGraphicalElements.size(); j++)
        {
            int oldObjectIndex = m_oldGraphicalElements[j]->GetID();
            if (currentObjectIndex == oldObjectIndex)
            {
                objectFound = true;
                //try to compare
                if (&m_graphicalObjects[i] == &m_oldGraphicalElements[j])
                {

                }
                else
                {
                    changeGeometriesIndices.push_back(oldObjectIndex);
                    break;
                }
            }
        }
        if (!objectFound) //add it, because it is probably new one
        {
            changeGeometriesIndices.push_back((int) i);
        }
    }
    return changeGeometriesIndices;
}

//int FrameWindow::FrameWindowImpl::GetUniqueId()
//{
//    //check the smallest not occupied index between all regions
//    for (int uniqueIndex = 0; uniqueIndex < 1000000; uniqueIndex++) // Should not be that many of controls
//    {
//        bool indexFound = false;
//        for (size_t graphicsIndex = 0; graphicsIndex < m_graphicalObjects.size(); graphicsIndex++)
//        {
//            if (m_graphicalObjects[graphicsIndex]->GetID() == uniqueIndex)
//            {
//                indexFound = true;
//                break;
//            }
//        }
//        if (!indexFound)
//        {
//            return uniqueIndex;
//        }
//    }
//    return 0;//should not go through this
//}

FrameWindow::FrameWindowImpl::~FrameWindowImpl()
{
    DeInit();
}

float FrameWindow::FrameWindowImpl::GetRescaleRatio()
{
    return m_latestPixelRatio;
}

void FrameWindow::FrameWindowImpl::DeleteGraphicalObjects(int groupId)
{
    std::unique_lock<std::mutex> lock(m_lock);
    for (size_t i = 0; i < m_graphicalObjects.size(); i++)
    {
        if (groupId == m_graphicalObjects[i]->GetGroup() || groupId == -1)
        {
            m_graphicalObjects[i]->DeInit();
            UnregisterGraphicalObject(m_graphicalObjects[i]);
            delete m_graphicalObjects[i];
            m_graphicalObjects[i] = nullptr;
        }
    }
    //take out empty graphical objects
    for (size_t i = 0; i < m_graphicalObjects.size(); i++)
    {
        if (m_graphicalObjects[i] == nullptr)
        {
            m_graphicalObjects.erase(m_graphicalObjects.begin() + i);
            i--;
        }
    }
}
unsigned int FrameWindow::FrameWindowImpl::RegisterGraphicalObject(GraphicalObject *graphicalObject)
{
    auto it = std::find_if(std::begin(m_graphicalObjectsMap), std::end(m_graphicalObjectsMap),
                           [](auto&& p) { return p.second == nullptr; });
    if (it == std::end(m_graphicalObjectsMap))
    {
        std::string exceptionMessage("Too many graphical elements, exceeded: %d", MAXGRAPHICALOBJECTS);
        throw std::out_of_range(exceptionMessage);
    }
    m_graphicalObjectsMap[it->first] = graphicalObject;
    return it->first;
}

void FrameWindow::FrameWindowImpl::UnregisterGraphicalObject(GraphicalObject *graphicalObject)
{
    auto it = std::find_if(std::begin(m_graphicalObjectsMap), std::end(m_graphicalObjectsMap),
                           [graphicalObject](auto&& p) { return p.second == graphicalObject; });
    if (it == std::end(m_graphicalObjectsMap))
    {
        std::string exceptionMessage("Graphical object with address not found in graphical object map!");
        throw std::invalid_argument(exceptionMessage);
    }
    m_graphicalObjectsMap[it->first] = nullptr;
}

void FrameWindow::FrameWindowImpl::DownscaleFrame(const Frame &src, Frame &dst, int nth)
{
    // calculate how many rows and column should be copied
    int columns = (int)src.width / nth;
    int rows = (int)src.height / nth;
    int channels = (int)src.channels;
    std::uint8_t* dstBuf = new std::uint8_t[columns * rows * channels];
    std::uint8_t* srcBuf = src.data.get();
    // Note that when image dimension is not divisible by 'nth',
    // the end of column and row should not be considered
    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < columns; x++)
        {
            int srcRow = y * nth;
            int srcColumn = x * nth;
            int srcIndex = (srcRow * (int)src.width + srcColumn) * channels;
            int dstIndex = (y * columns + x) * channels;
            for (int c = 0; c < channels; c++)
            {
                dstBuf[dstIndex + c] = srcBuf[srcIndex + c];
            }
        }
    }
    // recreate destination frame
    dst = Frame((std::uint32_t)columns, (std::uint32_t)rows, (std::uint8_t)channels,
                dstBuf, src.captureTime, src.index, src.providerName);
}
