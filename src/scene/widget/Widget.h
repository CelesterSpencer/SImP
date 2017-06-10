#ifndef SRCCMAKE_WIDGET_H
#define SRCCMAKE_WIDGET_H

class Widget
{
public:
    Widget()
    {
        m_priority = 5;
        m_shouldCleanUp = false;
    }

    virtual bool draw(int width, int height, float deltaTime) = 0;
    void setPriority(bool priority) { m_priority = priority; }
    bool getPriority() { return m_priority; }
    void setShouldCleanUp(bool shouldCleanUp) { m_shouldCleanUp = shouldCleanUp; }
    bool shouldCleanUp() { return m_shouldCleanUp; }

private:
    int m_priority;
    bool m_shouldCleanUp;
};

#endif //SRCCMAKE_WIDGET_H
