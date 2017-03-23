#ifndef SRCCMAKE_USERDATA_H
#define SRCCMAKE_USERDATA_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

class UserData
{
public:
    enum DataType
    {
        DT_INT   = 1,
        DT_FLOAT = 2,
        DT_BOOL  = 3,
        DT_IMAGE = 4,
        DT_OPTIONS = 5
    };

    UserData(std::string name, DataType dataType, void* p_data, void* p_min = nullptr, void* p_max = nullptr) {
        m_name = name;
        m_dataType = dataType;
        mp_data = p_data;
        mp_min = p_min;
        mp_max = p_max;
    }
    ~UserData() {}

    DataType m_dataType;
    std::string m_name;
    void* mp_data;
    void* mp_min;
    void* mp_max;
};

class UserDataCollection
{
public:
    void addUserData(UserData userData)
    {
        m_userDataVector.push_back(userData);
        m_nameIndexMap[userData.m_name] = m_userDataVector.size()-1;
    }
    UserData* getUserData(std::string name)
    {
        auto it = m_nameIndexMap.find(name);
        if(it != m_nameIndexMap.end())
        {
            return &m_userDataVector[it->second];
        }
        else
        {
            std::cerr << "Could not find user data of name " << name << std::endl;
            return nullptr;
        }
    }
    int getNumberOfElements() { return m_userDataVector.size(); }
    UserData* getUserDataAt(int index) { return  &m_userDataVector[index]; }
private:
    std::vector<UserData> m_userDataVector;
    std::map<std::string,int> m_nameIndexMap;
};

#endif //SRCCMAKE_USERDATA_H
