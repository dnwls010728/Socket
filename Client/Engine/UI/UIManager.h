#pragma once

namespace UI
{
    class Widget;

    class Manager : public Singleton<Manager>
    {
    public:
        Manager();
        virtual ~Manager() override = default;

        void Tick(float delta_time);
        void Render();
        void AddWidget(const std::shared_ptr<Widget>& widget);
        void RemoveWidget(const std::shared_ptr<Widget>& widget);

    private:
        friend class Core;

        void OnEvent(const Event& kEvent);
        
        std::vector<std::shared_ptr<Widget>> widgets_;
    
    };
}

