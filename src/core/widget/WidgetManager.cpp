#include "WidgetManager.h"

MenuTab::MenuTab(std::string tabName)
{
    m_name = tabName;
    m_isActive = true;
}
void MenuTab::draw()
{
    if (ImGui::BeginMenu(m_name.c_str(), m_isActive))
    {
        for(auto& entry : m_entries)
        {
            switch(entry->type)
            {
                case MenuTabEntry::EntryType::SELECTABLE:
                    if (ImGui::MenuItem(entry->name.c_str(), "", false, entry->isActive))
                        entry->action();
                    break;
                case MenuTabEntry::EntryType::TEXT:
                    ImGui::TextColored(entry->color, entry->name.c_str());
                    break;
                case MenuTabEntry::EntryType::DIVIDER:
                    ImGui::Separator();
                    break;
            }
        }
        ImGui::EndMenu();
    }
}
void MenuTab::addEntry(std::string entryName, std::function<void()> action, int priority, ImVec4 color, MenuTabEntry::EntryType type)
{
    m_entries.push_back(new MenuTabEntry(entryName, action, priority, color, true, type));
    m_nameEntryMap[entryName] = m_entries[m_entries.size()-1];
    std::stable_sort(m_entries.begin(), m_entries.end(), [](MenuTabEntry* a, MenuTabEntry* b) { return a->priority < b->priority; });
}
void MenuTab::enableEntry(std::string entryName)
{
    auto menuTabEntry = m_nameEntryMap.find(entryName);
    if(menuTabEntry != m_nameEntryMap.end())
        menuTabEntry->second->isActive = true;
}
void MenuTab::disableEntry(std::string entryName)
{
    auto menuTabEntry = m_nameEntryMap.find(entryName);
    if(menuTabEntry != m_nameEntryMap.end())
        menuTabEntry->second->isActive = false;
}



void WidgetManager::draw(int width, int height, float deltaTime)
{
    // draw menu tabs
    if(ImGui::BeginMainMenuBar())
    {
        for(auto& menuTab : m_menuTabs)
            menuTab->draw();

        ImGui::EndMainMenuBar();
    }

    // draw widgets
    std::vector<bool> widgetStati;
    for(auto& widget : m_widgets)
        widgetStati.push_back(widget->draw(width, height, deltaTime));
    // for all new created widgets return true
    for(int i = widgetStati.size(); i < m_widgets.size(); i++)
        widgetStati.push_back(true);

    // only copy over the valid widgets
    std::vector<Widget*> keptWidgets;
    for(int i = 0; i < widgetStati.size(); i++)
    {
        if(widgetStati[i]) keptWidgets.push_back(m_widgets[i]);
        else if(m_widgets[i]->shouldCleanUp()) delete m_widgets[i]; // cleanup widgets that are flagged for deletion
    }
    m_widgets = keptWidgets;
}
void WidgetManager::addMenuTab(std::string tabName, int priority)
{
    m_menuTabs.push_back(new MenuTab(tabName));
    m_nameMenuTabMap[tabName] = m_menuTabs[m_menuTabs.size()-1];
}
void WidgetManager::addMenuTabEntry(std::string tabName, std::string entryName,
                     std::function<void()> action, int priority, ImVec4 color)
{
    auto menuTab = m_nameMenuTabMap.find(tabName);
    if(menuTab != m_nameMenuTabMap.end())
        menuTab->second->addEntry(entryName, action, priority, color, MenuTabEntry::EntryType::SELECTABLE);
}
void WidgetManager::addMenuTabText(std::string tabName, std::string entryName, int priority, ImVec4 color)
{
    auto menuTab = m_nameMenuTabMap.find(tabName);
    if(menuTab != m_nameMenuTabMap.end())
        menuTab->second->addEntry(entryName, nullptr, priority, color, MenuTabEntry::EntryType::TEXT);
}
void WidgetManager::addMenuTabDivider(std::string tabName, int priority)
{
    auto menuTab = m_nameMenuTabMap.find(tabName);
    if(menuTab != m_nameMenuTabMap.end())
        menuTab->second->addEntry("divider"+std::to_string(priority),
                                  nullptr, priority, ImVec4(0.f,0.f,0.f,0.f), MenuTabEntry::EntryType::DIVIDER);
}

void WidgetManager::enableMenuTab(std::string tabName)
{
    auto menuTab = m_nameMenuTabMap.find(tabName);
    if(menuTab != m_nameMenuTabMap.end())
        menuTab->second->enable();
}
void WidgetManager::disableMenuTab(std::string tabName)
{
    auto menuTab = m_nameMenuTabMap.find(tabName);
    if(menuTab != m_nameMenuTabMap.end())
        menuTab->second->disable();
}
void WidgetManager::enableMenuTabEntry(std::string tabName, std::string entryName)
{
    auto menuTab = m_nameMenuTabMap.find(tabName);
    if(menuTab != m_nameMenuTabMap.end())
        menuTab->second->enableEntry(entryName);
}
void WidgetManager::disableMenuTabEntry(std::string tabName, std::string entryName)
{
    auto menuTab = m_nameMenuTabMap.find(tabName);
    if(menuTab != m_nameMenuTabMap.end())
        menuTab->second->disableEntry(entryName);
}

void WidgetManager::addWidget(Widget* p_widget, int priority, bool shouldCleanUp)
{
    p_widget->setPriority(priority);
    p_widget->setShouldCleanUp(shouldCleanUp);
    m_widgets.push_back(p_widget);
    std::sort(m_widgets.begin(), m_widgets.end(), [](Widget* a, Widget* b) -> bool
    {
        return a->getPriority() < b->getPriority();
    });
}