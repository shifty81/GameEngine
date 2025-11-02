#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H

#include <string>
#include <map>
#include <vector>
#include <functional>
#include <memory>
#include <iostream>

// Base Event class
class Event {
public:
    virtual ~Event() = default;
    virtual std::string GetType() const = 0;
    virtual std::string ToString() const { return GetType(); }
    
    bool Handled = false;
};

// Window Events
class WindowResizeEvent : public Event {
public:
    WindowResizeEvent(int width, int height) : Width(width), Height(height) {}
    
    std::string GetType() const override { return "WindowResize"; }
    std::string ToString() const override {
        return "WindowResize: " + std::to_string(Width) + "x" + std::to_string(Height);
    }
    
    int Width, Height;
};

class WindowCloseEvent : public Event {
public:
    std::string GetType() const override { return "WindowClose"; }
};

// Input Events
class KeyPressedEvent : public Event {
public:
    KeyPressedEvent(int keyCode, int repeatCount = 0) 
        : KeyCode(keyCode), RepeatCount(repeatCount) {}
    
    std::string GetType() const override { return "KeyPressed"; }
    std::string ToString() const override {
        return "KeyPressed: " + std::to_string(KeyCode) + " (repeats: " + std::to_string(RepeatCount) + ")";
    }
    
    int KeyCode;
    int RepeatCount;
};

class KeyReleasedEvent : public Event {
public:
    KeyReleasedEvent(int keyCode) : KeyCode(keyCode) {}
    
    std::string GetType() const override { return "KeyReleased"; }
    std::string ToString() const override {
        return "KeyReleased: " + std::to_string(KeyCode);
    }
    
    int KeyCode;
};

class MouseMovedEvent : public Event {
public:
    MouseMovedEvent(float x, float y) : X(x), Y(y) {}
    
    std::string GetType() const override { return "MouseMoved"; }
    std::string ToString() const override {
        return "MouseMoved: (" + std::to_string(X) + ", " + std::to_string(Y) + ")";
    }
    
    float X, Y;
};

class MouseButtonPressedEvent : public Event {
public:
    MouseButtonPressedEvent(int button) : Button(button) {}
    
    std::string GetType() const override { return "MouseButtonPressed"; }
    std::string ToString() const override {
        return "MouseButtonPressed: " + std::to_string(Button);
    }
    
    int Button;
};

class MouseButtonReleasedEvent : public Event {
public:
    MouseButtonReleasedEvent(int button) : Button(button) {}
    
    std::string GetType() const override { return "MouseButtonReleased"; }
    std::string ToString() const override {
        return "MouseButtonReleased: " + std::to_string(Button);
    }
    
    int Button;
};

class MouseScrolledEvent : public Event {
public:
    MouseScrolledEvent(float xOffset, float yOffset) 
        : XOffset(xOffset), YOffset(yOffset) {}
    
    std::string GetType() const override { return "MouseScrolled"; }
    std::string ToString() const override {
        return "MouseScrolled: (" + std::to_string(XOffset) + ", " + std::to_string(YOffset) + ")";
    }
    
    float XOffset, YOffset;
};

// Custom Game Events
class GameEvent : public Event {
public:
    GameEvent(const std::string& eventType, const std::string& data = "") 
        : EventType(eventType), Data(data) {}
    
    std::string GetType() const override { return EventType; }
    std::string ToString() const override {
        return EventType + ": " + Data;
    }
    
    std::string EventType;
    std::string Data;
};

// Event Dispatcher
class EventDispatcher {
public:
    template<typename T>
    bool Dispatch(Event& event, std::function<bool(T&)> func) {
        if (event.GetType() == T().GetType()) {
            event.Handled = func(static_cast<T&>(event));
            return true;
        }
        return false;
    }
};

// Event System - Central event bus
class EventSystem {
public:
    using EventCallback = std::function<void(Event&)>;
    
    // Singleton access
    static EventSystem* GetInstance() {
        static EventSystem instance;
        return &instance;
    }

    // Subscribe to events by type
    void Subscribe(const std::string& eventType, EventCallback callback) {
        m_Subscribers[eventType].push_back(callback);
    }

    // Subscribe to all events
    void SubscribeToAll(EventCallback callback) {
        m_GlobalSubscribers.push_back(callback);
    }

    // Publish an event immediately
    void PublishImmediate(Event& event) {
        // Call global subscribers
        for (auto& callback : m_GlobalSubscribers) {
            callback(event);
            if (event.Handled) break;
        }

        // Call type-specific subscribers
        auto it = m_Subscribers.find(event.GetType());
        if (it != m_Subscribers.end()) {
            for (auto& callback : it->second) {
                callback(event);
                if (event.Handled) break;
            }
        }

        if (m_LogEvents) {
            std::cout << "[Event] " << event.ToString() << std::endl;
        }
    }

    // Queue an event for later processing
    void QueueEvent(std::unique_ptr<Event> event) {
        m_EventQueue.push_back(std::move(event));
    }

    // Process all queued events
    void ProcessEvents() {
        for (auto& event : m_EventQueue) {
            PublishImmediate(*event);
        }
        m_EventQueue.clear();
    }

    // Enable/disable event logging
    void SetEventLogging(bool enabled) { m_LogEvents = enabled; }

    // Clear all subscribers
    void ClearSubscribers() {
        m_Subscribers.clear();
        m_GlobalSubscribers.clear();
    }

    // Get statistics
    size_t GetSubscriberCount(const std::string& eventType) const {
        auto it = m_Subscribers.find(eventType);
        return it != m_Subscribers.end() ? it->second.size() : 0;
    }

    size_t GetQueuedEventCount() const {
        return m_EventQueue.size();
    }

private:
    EventSystem() : m_LogEvents(false) {}
    ~EventSystem() {}

    // Prevent copying
    EventSystem(const EventSystem&) = delete;
    EventSystem& operator=(const EventSystem&) = delete;

    std::map<std::string, std::vector<EventCallback>> m_Subscribers;
    std::vector<EventCallback> m_GlobalSubscribers;
    std::vector<std::unique_ptr<Event>> m_EventQueue;
    bool m_LogEvents;
};

// Convenience macros for event handling
#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#endif // EVENTSYSTEM_H
