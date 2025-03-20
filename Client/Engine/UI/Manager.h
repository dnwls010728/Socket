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

    private:
        std::vector<std::shared_ptr<UI::Widget>> widgets_;
    
    };
}

