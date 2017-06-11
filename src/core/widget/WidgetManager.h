#ifndef SRCCMAKE_WIDGETMANAGER_H
#define SRCCMAKE_WIDGETMANAGER_H

// std
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <map>

// lib
#include <glm.hpp>
#include <imgui.h>

// project
#include "Widget.h"

class strless {
public:
    bool operator() (const std::string & first, const std::string & second ) const  {
        return first < second;
    }
};

struct MenuTabEntry
{
    enum EntryType { SELECTABLE = 1, TEXT = 2, DIVIDER = 3 };

    MenuTabEntry(std::string name, std::function<void()> action, int priority, ImVec4 color, bool isActive, EntryType type)
    {
        this->name = name;
        this->action = action;
        this->priority = priority;
        this->color = color;
        this->isActive = isActive;
        this->type = type;
    }

    std::string name;
    std::function<void()> action;
    int priority;
    ImVec4 color;
    bool isActive;
    EntryType type;
};

class MenuTab
{
public:
    MenuTab(std::string tabName);
    void draw();
    void addEntry(std::string entryName, std::function<void()> action, int priority, ImVec4 color, MenuTabEntry::EntryType type);
    void enableEntry(std::string entryName);
    void disableEntry(std::string entryName);
    void enable() { m_isActive = true; }
    void disable() { m_isActive = false; }

private:
    std::string m_name;
    bool m_isActive;
    std::vector<MenuTabEntry*> m_entries;
    std::map<std::string, MenuTabEntry*, strless> m_nameEntryMap;
};

class WidgetManager
{
public:
    void draw(int width, int height, float deltaTime);
    void addMenuTab(std::string tabName, int priority);
    void addMenuTabEntry(std::string tabName, std::string entryName,
                         std::function<void()> action, int priority = 5, ImVec4 color = ImVec4(0.f, 0.f, 0.f, 1.f));
    void addMenuTabText(std::string tabName, std::string entryName, int priority = 5, ImVec4 color = ImVec4(0.f, 0.f, 0.f, 1.f));
    void addMenuTabDivider(std::string tabName, int priority = 5);

    void enableMenuTab(std::string tabName);
    void disableMenuTab(std::string tabName);
    void enableMenuTabEntry(std::string tabName, std::string entryName);
    void disableMenuTabEntry(std::string tabName, std::string entryName);

    void addWidget(Widget* p_widget, int priority, bool shouldCleanUp = false);

private:
    std::vector<Widget*> m_widgets;
    std::vector<MenuTab*> m_menuTabs;
    std::map<std::string, MenuTab*, strless> m_nameMenuTabMap;
};

#endif //SRCCMAKE_WIDGETMANAGER_H