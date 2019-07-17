#include <Nero/editor/Interface.h>
#include <SFML/Window/Event.hpp>
#include <Nero/core/engine/EngineConstant.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <Nero/core/utility/FileUtil.h>
#include <nativefiledialog/include/nfd.h>

namespace  nero
{
    Interface::Interface(sf::RenderWindow& window):
        m_RenderWindow(window)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        project_manager_texture.loadFromFile("editor_project_manager.png");

        m_ProjectManager = std::make_unique<ProjectManager>();
        m_SceneManager = std::make_unique<SceneManager>();

    }

    Interface::~Interface()
    {
       //Empty
    }

    void Interface::handleEvent(const sf::Event& event)
    {
        if(event.type == sf::Event::Closed)
        {
            quitEditor();
        }

        ImGui::SFML::ProcessEvent(event);
    }

    void Interface::update(const sf::Time& timeStep)
    {

    }

    void Interface::render()
    {
        ImGui::SFML::Update(m_RenderWindow, EngineConstant.TIME_PER_FRAME);

        createDockSpace();

        ImGui::ShowDemoWindow();

        auto node = ImGui::DockBuilderGetNode(actionBarId);
        //node->SizeRef.x = 652;
        node->SizeRef.y = 20;

        ImGui::SFML::Render(m_RenderWindow);
    }

    void Interface::updateFrameRate(const float& frameRate, const float& frameTime)
    {

    }

    void Interface::quitEditor()
    {
        m_RenderWindow.close();
    }

    void Interface::createDockSpace()
    {
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", nullptr, window_flags);

        ImGui::PopStyleVar();

        ImGui::PopStyleVar(2);

        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

        ImGui::End();

        //ImGuiDir_Left
        //ImGuiDir_Up;
        //ImGuiDir_Down;
        //ImGuiDir_Right;
        //ImGuiDir_None;

        if(!setup_dock && !fileExist(getPath({"imgui"}, ".ini")))
        {
            ImGuiID dock_main_id = dockspace_id; // This variable will track the document node, however we are not using it here as we aren't docking anything into it.
            //ImGuiID dock_id_prop = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, nullptr, &dock_main_id);
            //ImGuiID dock_id_bottom = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.20f, nullptr, &dock_main_id);

            /*ImGui::DockBuilderDockWindow("Logfff", dock_id_bottom);
            ImGui::DockBuilderDockWindow("Properties", dock_id_prop);
            ImGui::DockBuilderDockWindow("Mesh", dock_id_prop);
            ImGui::DockBuilderDockWindow("Extra", dock_id_prop);
            ImGui::DockBuilderFinish(dockspace_id);*/

            //split main dock in five
            //ImGuiID dock_id_center = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_None, 0.20f, nullptr, &dock_main_id);
            ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, nullptr, &dock_main_id);
            ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.20f, nullptr, &dock_main_id);
            actionBarId = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.20f, nullptr, &dock_main_id);
            ImGuiID dock_id_down = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.20f, nullptr, &dock_main_id);

            //ImGui::DockBuilderGetNode(dock_id_left)->LocalFlags = ImGuiDockNodeFlags_AutoHideTabBar | ImGuiDockNodeFlags_NoResize | ImGuiDockNodeFlags_KeepAliveOnly;
            //ImGui::DockBuilderGetNode(dock_id_up)->LocalFlags = ImGuiDockNodeFlags_KeepAliveOnly | ImGuiDockNodeFlags_NoResize | ImGuiDockNodeFlags_NoSplit  | ImGuiDockNodeFlags_AutoHideTabBar;

            ImGui::DockBuilderDockWindow("main_center", ImGui::DockBuilderGetCentralNode(dockspace_id)->ID);
            ImGui::DockBuilderDockWindow("main_left", dock_id_left);
            ImGui::DockBuilderDockWindow("main_right", dock_id_right);
            ImGui::DockBuilderDockWindow("Dear ImGui Demo", dock_id_right);
            ImGui::DockBuilderDockWindow("main_up", actionBarId);
            ImGui::DockBuilderDockWindow("main_down", dock_id_down);
            ImGui::DockBuilderFinish(dockspace_id);

            auto node = ImGui::DockBuilderGetNode(actionBarId);
            node->SizeRef.x = 652;
            node->SizeRef.y = 20;
            node->LocalFlags = ImGuiDockNodeFlags_AutoHideTabBar | ImGuiDockNodeFlags_NoSplit |  ImGuiDockNodeFlags_NoResize | ImGuiDockNodeFlags_SingleDock;
            setup_dock = true;
        }


        ImGui::Begin("main_center");
            ImGui::Button("main_center_button");
        ImGui::End();

        window_flags = ImGuiWindowFlags_None;
        window_flags |= ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;
        ImGui::Begin("main_left");
             ImGui::Button("main_left_button");
        ImGui::End();

        ImGui::Begin("main_right");
             ImGui::Button("main_right_button");
        ImGui::End();

        //ImGui::SetNextWindowSizeConstraints(ImVec2(652, 38), ImVec2(1000, 40), nullptr, nullptr);

        ImGui::Begin("main_up", nullptr, window_flags);
             ImGui::Columns(5, "mixed", false);

             ImGui::NextColumn();

             ImGui::NextColumn();

             ImGui::NextColumn();

             ImGui::NextColumn();


             //create project button
             ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
             ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.0f, 0.7f, 0.7f));
             ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.0f, 0.8f, 0.8f));
             if(ImGui::Button("Project", ImVec2(100.f, 0.f)))
             {

                //show_project_window = true;
                 ImGui::OpenPopup("Project Manager");

             }
             ImGui::PopStyleColor(3);
             showProjectManagerWindow();


             ImGui::Columns(1);

        ImGui::End();

        ImGui::Begin("main_down");
             ImGui::Button("main_down_button");
        ImGui::End();

        /*if(show_project_window)
        {
            showProjectManagerWindow();
        }*/

    }

    void Interface::showProjectManagerWindow()
    {
        //build window flags
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal |
                                        ImGuiWindowFlags_NoResize |
                                        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;

        //compute and set window size and position
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 size(800.f, 500.f); //600-400 or 800-500
        ImVec2 position;
        position.x = (viewport->Size.x - size.x)/2.f;
        position.y = (viewport->Size.y - size.y)/2.f;
        ImGui::SetNextWindowSize(size);
        ImGui::SetNextWindowPos(position);

        //project manager window
        if(ImGui::BeginPopupModal("Project Manager", nullptr, window_flags))
        {

            //save cursor position
            ImVec2 cursor = ImGui::GetCursorPos();

            //nero_log(std::to_string(ImGui::GetWindowContentRegionWidth() * 0.33f));
            //Panel 1 : left illustration
            ImGui::BeginChild("Child1", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.33f, size.y - 20.f));
                ImGui::Image(project_manager_texture);
            ImGui::EndChild();

            //Panel 2 : create project form
            ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() * 0.33f);
            ImGui::SetCursorPosY(cursor.y);
            ImGui::BeginChild("Create Project", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.67f, size.y * 0.7f));

                ImGui::Text("Create Project");
                ImGui::Separator();
                ImGui::Dummy(ImVec2(0.0f, 5.0f));

                ImGui::Text("Create a new Project and start a new Adventure");
                ImGui::Dummy(ImVec2(0.0f, 5.0f));

                ImGui::BeginChild("project form");

                    float wording_width = 150.f;
                    float input_width = ImGui::GetWindowContentRegionWidth() - 150.f;

                    ImGui::Text("Project Name");
                    ImGui::SameLine(wording_width);
                    static char project_name[50] = "";
                    ImGui::SetNextItemWidth(input_width);
                    ImGui::InputText("##project_name", project_name, IM_ARRAYSIZE(project_name));
                    ImGui::Dummy(ImVec2(0.0f, 2.0f));


                    std::string default_value = m_EditorSetting["project_lead"].get<std::string>();
                    int array_size = default_value.length() < 50 ? 50 : default_value.length();
                    ImGui::Text("Project Lead");
                    ImGui::SameLine(wording_width);
                    char project_lead[array_size];
                    std::strcpy(project_lead, default_value.c_str());
                    ImGui::SetNextItemWidth(input_width);
                    ImGui::InputText("##project_lead", project_lead, IM_ARRAYSIZE(project_lead));
                    ImGui::Dummy(ImVec2(0.0f, 2.0f));

                    default_value = m_EditorSetting["project_company"].get<std::string>();
                    array_size = default_value.length() < 50 ? 50 : default_value.length();
                    ImGui::Text("Company Name");
                    ImGui::SameLine(wording_width);
                    char project_company[array_size];
                    std::strcpy(project_company, default_value.c_str());
                    ImGui::SetNextItemWidth(input_width);
                    ImGui::InputText("##project_company", project_company, IM_ARRAYSIZE(project_company));
                    ImGui::Dummy(ImVec2(0.0f, 2.0f));

                    default_value = m_EditorSetting["project_description"].get<std::string>();
                    array_size = default_value.length() < 400 ? 400 : default_value.length();
                    ImGui::Text("Description");
                    ImGui::SameLine(wording_width);
                    char project_description[array_size];
                    std::strcpy(project_description, default_value.c_str());
                    static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
                    ImGui::InputTextMultiline("##project_description", project_description, IM_ARRAYSIZE(project_description), ImVec2(input_width, ImGui::GetTextLineHeight() * 5), flags);
                    ImGui::Dummy(ImVec2(0.0f, 5.0f));

                    ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 102.f);
                    bool onCreate = ImGui::Button("Create", ImVec2(100, 0));
                    std::string error_message = StringPool.BLANK;
                    bool error = true;

                    if(std::string(project_name) == StringPool.BLANK)
                    {
                        error_message = "Please enter a Project Name";
                    }
                    else if(m_ProjectManager->isProjectExist(std::string(project_name)))
                    {
                        error_message = "A project with the same signature already exist,\n"
                                        "please choose another Project Name";
                    }
                    else if(std::string(project_lead) == StringPool.BLANK)
                    {
                        error_message = "Please enter a Project Lead";
                    }
                    else if (std::string(project_company) == StringPool.BLANK)
                    {
                        error_message = "Please enter a Company Name";
                    }
                    else
                    {
                        error = false;
                    }

                    if (onCreate && error)
                    {
                        ImGui::OpenPopup("Error Creating Project");
                    }
                    else if(onCreate)
                    {
                        nlohmann::json projectJson;

                        projectJson["project_name"]         = std::string(project_name);
                        projectJson["project_lead"]         = std::string(project_lead);
                        projectJson["project_company"]      = std::string(project_company);
                        projectJson["project_description"]  = std::string(project_description);

                        memset(project_name, 0, sizeof project_name);
                        memset(project_lead, 0, sizeof project_lead);
                        memset(project_company, 0, sizeof project_company);
                        memset(project_description, 0, sizeof project_company);

                        createProject(projectJson);

                        ImGui::CloseCurrentPopup();
                    }

                    ImGui::SetNextWindowSize(ImVec2(300.f, 120.f));
                    if(ImGui::BeginPopupModal("Error Creating Project", nullptr, window_flags))
                    {
                        ImGui::Text("%s", error_message.c_str());
                        ImGui::Dummy(ImVec2(0.0f, 45.0f));
                        ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 95.f);
                        if (ImGui::Button("Close", ImVec2(100, 0)))
                        {
                            ImGui::CloseCurrentPopup();
                        }

                        ImGui::EndPopup();
                    }

                ImGui::EndChild();
            ImGui::EndChild();

            //Panel 3 : existing project list
            ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() * 0.33f);
            ImGui::SetCursorPosY(cursor.y + size.y * 0.5f);
            ImGui::BeginChild("Open Project", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.67f, size.y * 0.5f - 75.f));

            ImGui::Text("Open Project");
            ImGui::Separator();

            ImGui::Dummy(ImVec2(0.0f, 5.0f));
            ImGui::BeginChild("project list", ImVec2(0.f, 0.f), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

                for(const nlohmann::json& project : m_ProjectManager->getProjectTable())
                {
                    std::string project_name = project["project_name"].get<std::string>();

                    if (ImGui::CollapsingHeader(project_name.c_str()))
                    {
                                    project_name        = ": " + project_name;
                        std::string project_id          = ": " + project["project_id"].get<std::string>();
                        std::string project_lead        = ": " + project["project_lead"].get<std::string>();
                        std::string project_company     = ": " + project["project_company"].get<std::string>();
                        std::string project_description = ": " + project["project_description"].get<std::string>();
                        std::string creation_date       = ": " + project["creation_date"].get<std::string>();
                        std::string modification_date   = ": " + project["modification_date"].get<std::string>();

                        ImGui::Text("Project Name");
                        ImGui::SameLine(wording_width);
                        ImGui::Text(project_name.c_str());
                        ImGui::Dummy(ImVec2(0.0f, 2.0f));

                        ImGui::Text("Project Id");
                        ImGui::SameLine(wording_width);
                        ImGui::Text(project_id.c_str());
                        ImGui::Dummy(ImVec2(0.0f, 2.0f));

                        ImGui::Text("Project Lead");
                        ImGui::SameLine(wording_width);
                        ImGui::Text(project_lead.c_str());
                        ImGui::Dummy(ImVec2(0.0f, 2.0f));

                        ImGui::Text("Project Company");
                        ImGui::SameLine(wording_width);
                        ImGui::Text(project_company.c_str());
                        ImGui::Dummy(ImVec2(0.0f, 2.0f));

                        ImGui::Text("Create Date");
                        ImGui::SameLine(wording_width);
                        ImGui::Text(creation_date.c_str());
                        ImGui::Dummy(ImVec2(0.0f, 2.0f));

                        ImGui::Text("Modification Date");
                        ImGui::SameLine(wording_width);
                        ImGui::Text(modification_date.c_str());
                        ImGui::Dummy(ImVec2(0.0f, 2.0f));

                        ImGui::Text("Project Description");
                        ImGui::SameLine(wording_width);
                        ImGui::Text(project_description.c_str());
                        ImGui::Dummy(ImVec2(0.0f, 5.0f));


                        ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 100.f);
                        if (ImGui::Button("Open Project", ImVec2(100, 0)))
                        {
                            openProject(project);

                            ImGui::CloseCurrentPopup();
                        }

                        ImGui::Dummy(ImVec2(0.0f, 10.0f));

                    }
                }
             ImGui::EndChild();

            ImGui::EndChild();

            ImGui::Dummy(ImVec2(0.0f, 7.0f));

            cursor = ImGui::GetCursorPos();
            ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() * 0.33f);
            if (ImGui::Button("Open Folder", ImVec2(100, 0)))
            {
                nfdchar_t *outPath = nullptr;
                nfdresult_t result = NFD_OpenDialog( nullptr, nullptr, &outPath );

                if ( result == NFD_OKAY ) {
                    puts("Success!");
                    puts(outPath);
                    free(outPath);

                    nero_log(outPath);

                    openProject(std::string(outPath));
                }
                else if ( result == NFD_CANCEL ) {
                    puts("User pressed cancel.");
                }
                else {
                    printf("Error: %s\n", NFD_GetError() );
                }

                //ImGui::CloseCurrentPopup();
            }

            ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 102.f);
            ImGui::SetCursorPosY(cursor.y);
            if (ImGui::Button("Close", ImVec2(100, 0)))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void Interface::addScene(const std::string& projectName, std::function<Scene::Ptr(Scene::Context)> factory)
    {
        //Engine SDK create and open a C++ project [Project_C++]

        if(m_ProjectManager->isProjectExist(projectName))
        {
            m_SceneManager->addSceneFacotry(formatString(projectName), factory);
        }
        else
        {
            m_ProjectManager->createProject(projectName, ProjectManager::CPP_PROJECT);
            m_SceneManager->addSceneFacotry(formatString(projectName), factory);

            //save project
            //load project
        }
    }

    void Interface::addLuaScene(const std::string& projectName, std::function<LuaScene::Ptr(Scene::Context)> factory)
    {
        //Engine SDK create or open a C++/Lua project [Project_C++_LUA]

        if(m_ProjectManager->isProjectExist(projectName))
        {
            m_SceneManager->addLuaSceneFacotry(formatString(projectName), factory);
        }
        else
        {
            m_ProjectManager->createProject(projectName, ProjectManager::LUA_CPP_PROJECT);
            m_SceneManager->addLuaSceneFacotry(formatString(projectName), factory);
        }
    }

    void Interface::createProject(const nlohmann::json& projectJson)
    {
        m_ProjectManager->createProject(projectJson, ProjectManager::LUA_PROJECT);
    }

    void Interface::openProject(const std::string& path)
    {
        //Open a Lua project or Select a C++, C++/Lua project
    }

    void Interface::setEditorSetting(const nlohmann::json& setting)
    {
        m_EditorSetting = setting;

        m_ProjectManager->setEditorSetting(m_EditorSetting);
    }

    void Interface::loadAllProject()
    {
        m_ProjectManager->loadAllProject();
    }

    void Interface::openProject(const nlohmann::json& projectJson)
    {

    }


}