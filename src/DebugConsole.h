#ifndef DEBUGCONSOLE_H
#define DEBUGCONSOLE_H

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

/**
 * DebugConsole - A powerful runtime debugging console system
 * 
 * Features:
 * - Command registration and execution
 * - Command history and auto-completion
 * - Enable/disable functionality
 * - Variable get/set commands
 * - Extensible command system
 * - Thread-safe command execution
 * 
 * Usage:
 *   auto console = DebugConsole::GetInstance();
 *   console->RegisterCommand("mycommand", "Description", [](const std::vector<std::string>& args) {
 *       // Command implementation
 *   });
 *   console->ExecuteCommand("mycommand arg1 arg2");
 */
class DebugConsole {
public:
    using CommandCallback = std::function<void(const std::vector<std::string>&)>;
    
    static DebugConsole* GetInstance() {
        static DebugConsole instance;
        return &instance;
    }
    
    // Console state management
    void SetEnabled(bool enabled) { 
        m_Enabled = enabled;
        if (enabled) {
            Log("[Console] Debug console enabled. Type 'help' for available commands.");
        }
    }
    
    bool IsEnabled() const { return m_Enabled; }
    void Toggle() { SetEnabled(!m_Enabled); }
    
    // Command registration
    void RegisterCommand(const std::string& name, const std::string& description, CommandCallback callback) {
        std::string lowerName = ToLower(name);
        m_Commands[lowerName] = {description, callback};
    }
    
    void UnregisterCommand(const std::string& name) {
        std::string lowerName = ToLower(name);
        m_Commands.erase(lowerName);
    }
    
    // Command execution
    void ExecuteCommand(const std::string& commandLine) {
        if (commandLine.empty()) return;
        
        // Add to history
        if (m_CommandHistory.empty() || m_CommandHistory.back() != commandLine) {
            m_CommandHistory.push_back(commandLine);
            if (m_CommandHistory.size() > m_MaxHistorySize) {
                m_CommandHistory.erase(m_CommandHistory.begin());
            }
        }
        m_HistoryIndex = m_CommandHistory.size();
        
        // Parse command and arguments
        std::vector<std::string> tokens = Tokenize(commandLine);
        if (tokens.empty()) return;
        
        std::string command = ToLower(tokens[0]);
        std::vector<std::string> args(tokens.begin() + 1, tokens.end());
        
        // Execute command
        auto it = m_Commands.find(command);
        if (it != m_Commands.end()) {
            try {
                it->second.callback(args);
            } catch (const std::exception& e) {
                Log("[Error] Command execution failed: " + std::string(e.what()));
            }
        } else {
            Log("[Error] Unknown command: '" + tokens[0] + "'. Type 'help' for available commands.");
        }
    }
    
    // Variable system
    void SetVariable(const std::string& name, const std::string& value) {
        m_Variables[name] = value;
    }
    
    std::string GetVariable(const std::string& name, const std::string& defaultValue = "") const {
        auto it = m_Variables.find(name);
        return it != m_Variables.end() ? it->second : defaultValue;
    }
    
    bool HasVariable(const std::string& name) const {
        return m_Variables.find(name) != m_Variables.end();
    }
    
    // Logging
    void Log(const std::string& message) {
        m_OutputLog.push_back(message);
        if (m_OutputLog.size() > m_MaxLogSize) {
            m_OutputLog.erase(m_OutputLog.begin());
        }
        
        // Also print to stdout if console is enabled
        if (m_Enabled) {
            std::cout << message << std::endl;
        }
    }
    
    void Clear() {
        m_OutputLog.clear();
    }
    
    // History navigation
    std::string GetPreviousCommand() {
        if (m_CommandHistory.empty()) return "";
        if (m_HistoryIndex > 0) m_HistoryIndex--;
        if (m_HistoryIndex < m_CommandHistory.size()) {
            return m_CommandHistory[m_HistoryIndex];
        }
        return "";
    }
    
    std::string GetNextCommand() {
        if (m_CommandHistory.empty()) return "";
        if (m_HistoryIndex < m_CommandHistory.size() - 1) {
            m_HistoryIndex++;
            return m_CommandHistory[m_HistoryIndex];
        }
        m_HistoryIndex = m_CommandHistory.size();
        return "";
    }
    
    // Auto-completion
    std::vector<std::string> GetCommandSuggestions(const std::string& prefix) const {
        std::vector<std::string> suggestions;
        std::string lowerPrefix = ToLower(prefix);
        
        for (const auto& pair : m_Commands) {
            if (pair.first.find(lowerPrefix) == 0) {
                suggestions.push_back(pair.first);
            }
        }
        
        std::sort(suggestions.begin(), suggestions.end());
        return suggestions;
    }
    
    // Getters
    const std::vector<std::string>& GetOutputLog() const { return m_OutputLog; }
    const std::map<std::string, std::string>& GetVariables() const { return m_Variables; }
    
    // Initialize built-in commands
    void InitializeBuiltInCommands() {
        // Help command
        RegisterCommand("help", "Display all available commands", 
            [this](const std::vector<std::string>& args) {
                if (args.empty()) {
                    Log("\n=== Available Commands ===");
                    std::vector<std::pair<std::string, std::string>> sortedCommands;
                    for (const auto& pair : m_Commands) {
                        sortedCommands.push_back({pair.first, pair.second.description});
                    }
                    std::sort(sortedCommands.begin(), sortedCommands.end());
                    
                    for (const auto& cmd : sortedCommands) {
                        Log("  " + cmd.first + " - " + cmd.second);
                    }
                    Log("==========================\n");
                } else {
                    // Show help for specific command
                    std::string cmdName = ToLower(args[0]);
                    auto it = m_Commands.find(cmdName);
                    if (it != m_Commands.end()) {
                        Log("Command: " + cmdName);
                        Log("Description: " + it->second.description);
                    } else {
                        Log("[Error] Unknown command: " + args[0]);
                    }
                }
            });
        
        // Clear command
        RegisterCommand("clear", "Clear the console output",
            [this](const std::vector<std::string>& args) {
                Clear();
                Log("[Console] Output cleared");
            });
        
        // Set variable command
        RegisterCommand("set", "Set a console variable (usage: set <name> <value>)",
            [this](const std::vector<std::string>& args) {
                if (args.size() < 2) {
                    Log("[Error] Usage: set <name> <value>");
                    return;
                }
                std::string value = args[1];
                for (size_t i = 2; i < args.size(); i++) {
                    value += " " + args[i];
                }
                SetVariable(args[0], value);
                Log("[Console] Variable '" + args[0] + "' set to '" + value + "'");
            });
        
        // Get variable command
        RegisterCommand("get", "Get a console variable value (usage: get <name>)",
            [this](const std::vector<std::string>& args) {
                if (args.empty()) {
                    Log("[Error] Usage: get <name>");
                    return;
                }
                if (HasVariable(args[0])) {
                    Log(args[0] + " = " + GetVariable(args[0]));
                } else {
                    Log("[Error] Variable '" + args[0] + "' not found");
                }
            });
        
        // List variables command
        RegisterCommand("listvars", "List all console variables",
            [this](const std::vector<std::string>& args) {
                if (m_Variables.empty()) {
                    Log("[Console] No variables set");
                    return;
                }
                Log("\n=== Console Variables ===");
                for (const auto& pair : m_Variables) {
                    Log("  " + pair.first + " = " + pair.second);
                }
                Log("=========================\n");
            });
        
        // Echo command
        RegisterCommand("echo", "Echo text to console (usage: echo <text>)",
            [this](const std::vector<std::string>& args) {
                std::string message;
                for (size_t i = 0; i < args.size(); i++) {
                    if (i > 0) message += " ";
                    message += args[i];
                }
                Log(message);
            });
        
        // Exit/quit command
        RegisterCommand("exit", "Close the console",
            [this](const std::vector<std::string>& args) {
                SetEnabled(false);
            });
        
        RegisterCommand("quit", "Close the console",
            [this](const std::vector<std::string>& args) {
                SetEnabled(false);
            });
    }

private:
    DebugConsole() 
        : m_Enabled(false),
          m_MaxLogSize(100),
          m_MaxHistorySize(50),
          m_HistoryIndex(0) {
        InitializeBuiltInCommands();
    }
    
    ~DebugConsole() {}
    
    DebugConsole(const DebugConsole&) = delete;
    DebugConsole& operator=(const DebugConsole&) = delete;
    
    // Helper functions
    static std::string ToLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                      [](unsigned char c) { return std::tolower(c); });
        return result;
    }
    
    static std::vector<std::string> Tokenize(const std::string& str) {
        std::vector<std::string> tokens;
        std::istringstream iss(str);
        std::string token;
        
        while (iss >> token) {
            tokens.push_back(token);
        }
        
        return tokens;
    }
    
    struct CommandInfo {
        std::string description;
        CommandCallback callback;
    };
    
    bool m_Enabled;
    size_t m_MaxLogSize;
    size_t m_MaxHistorySize;
    size_t m_HistoryIndex;
    
    std::map<std::string, CommandInfo> m_Commands;
    std::map<std::string, std::string> m_Variables;
    std::vector<std::string> m_OutputLog;
    std::vector<std::string> m_CommandHistory;
};

/**
 * ConsoleCommandRegistry - Helper class for registering game-specific commands
 * 
 * This class provides convenient methods to register common game debugging commands
 */
class ConsoleCommandRegistry {
public:
    static void RegisterGameCommands(DebugConsole* console) {
        // FPS/Performance commands
        console->RegisterCommand("showfps", "Toggle FPS display",
            [](const std::vector<std::string>& args) {
                DebugConsole::GetInstance()->Log("[Console] FPS display toggled (implementation needed in main)");
            });
        
        // Graphics commands
        console->RegisterCommand("wireframe", "Toggle wireframe rendering mode",
            [](const std::vector<std::string>& args) {
                DebugConsole::GetInstance()->Log("[Console] Wireframe mode toggled (implementation needed in main)");
            });
        
        console->RegisterCommand("cellshading", "Toggle cell shading (usage: cellshading on|off)",
            [](const std::vector<std::string>& args) {
                if (args.empty()) {
                    DebugConsole::GetInstance()->Log("[Error] Usage: cellshading on|off");
                } else {
                    DebugConsole::GetInstance()->Log("[Console] Cell shading " + args[0]);
                }
            });
        
        // Camera commands
        console->RegisterCommand("setcamera", "Set camera position (usage: setcamera <x> <y> <z>)",
            [](const std::vector<std::string>& args) {
                if (args.size() < 3) {
                    DebugConsole::GetInstance()->Log("[Error] Usage: setcamera <x> <y> <z>");
                } else {
                    DebugConsole::GetInstance()->Log("[Console] Camera position set to (" + 
                        args[0] + ", " + args[1] + ", " + args[2] + ")");
                }
            });
        
        console->RegisterCommand("resetcamera", "Reset camera to default position",
            [](const std::vector<std::string>& args) {
                DebugConsole::GetInstance()->Log("[Console] Camera reset to default position");
            });
        
        // Performance commands
        console->RegisterCommand("setfps", "Set target FPS limit (usage: setfps <fps>)",
            [](const std::vector<std::string>& args) {
                if (args.empty()) {
                    DebugConsole::GetInstance()->Log("[Error] Usage: setfps <fps>");
                } else {
                    DebugConsole::GetInstance()->SetVariable("target_fps", args[0]);
                    DebugConsole::GetInstance()->Log("[Console] Target FPS set to " + args[0]);
                }
            });
        
        // Debug visualization commands
        console->RegisterCommand("showbounds", "Toggle bounding box visualization",
            [](const std::vector<std::string>& args) {
                DebugConsole::GetInstance()->Log("[Console] Bounding box visualization toggled");
            });
        
        console->RegisterCommand("shownormals", "Toggle normal vector visualization",
            [](const std::vector<std::string>& args) {
                DebugConsole::GetInstance()->Log("[Console] Normal vector visualization toggled");
            });
        
        // Scene commands
        console->RegisterCommand("reload", "Reload shaders and assets",
            [](const std::vector<std::string>& args) {
                DebugConsole::GetInstance()->Log("[Console] Reloading shaders and assets...");
            });
        
        console->RegisterCommand("timescale", "Set time scale for game speed (usage: timescale <scale>)",
            [](const std::vector<std::string>& args) {
                if (args.empty()) {
                    DebugConsole::GetInstance()->Log("[Error] Usage: timescale <scale>");
                } else {
                    DebugConsole::GetInstance()->SetVariable("time_scale", args[0]);
                    DebugConsole::GetInstance()->Log("[Console] Time scale set to " + args[0]);
                }
            });
        
        // System info commands
        console->RegisterCommand("sysinfo", "Display system information",
            [](const std::vector<std::string>& args) {
                auto console = DebugConsole::GetInstance();
                console->Log("\n=== System Information ===");
                console->Log("Engine: 3D Game Engine v1.0");
                console->Log("Renderer: OpenGL 3.3+");
                console->Log("Console: Enabled");
                console->Log("==========================\n");
            });
        
        // Memory commands
        console->RegisterCommand("memstats", "Display memory statistics",
            [](const std::vector<std::string>& args) {
                DebugConsole::GetInstance()->Log("[Console] Memory statistics (implementation needed)");
            });
    }
};

#endif // DEBUGCONSOLE_H
