#pragma once

class ProjectSettings : public Singleton<ProjectSettings>
{
public:
    ProjectSettings();
    virtual ~ProjectSettings() override = default;

    void Init();
    
};
