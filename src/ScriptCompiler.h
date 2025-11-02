#ifndef SCRIPTCOMPILER_H
#define SCRIPTCOMPILER_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <iostream>
#include <sstream>
#include <fstream>

/**
 * ScriptCompiler - Custom compiler system for runtime script compilation
 * 
 * This system allows the game engine to compile and execute custom scripts
 * at runtime without requiring C++ recompilation. It provides a simple
 * scripting language that can interact with engine systems.
 * 
 * Features:
 * - Runtime compilation of script files
 * - Variable storage and retrieval
 * - Function registration from C++ code
 * - Simple expression evaluation
 * - Command execution system
 * 
 * Example Usage:
 *   ScriptCompiler compiler;
 *   compiler.RegisterFunction("print", [](const std::vector<std::string>& args) {
 *       for (const auto& arg : args) std::cout << arg << " ";
 *       std::cout << std::endl;
 *   });
 *   compiler.CompileAndExecute("print Hello World");
 */
class ScriptCompiler {
public:
    using ScriptFunction = std::function<void(const std::vector<std::string>&)>;
    
    ScriptCompiler() {
        // Register built-in functions
        RegisterBuiltInFunctions();
    }
    
    /**
     * Compile and execute a script string
     * @param script The script code to compile and execute
     * @return true if compilation and execution succeeded
     */
    bool CompileAndExecute(const std::string& script) {
        if (script.empty()) {
            return false;
        }
        
        // Tokenize the script
        std::vector<std::string> tokens = Tokenize(script);
        if (tokens.empty()) {
            return false;
        }
        
        // Execute the tokens
        return Execute(tokens);
    }
    
    /**
     * Compile and execute a script file
     * @param filename Path to the script file
     * @return true if compilation and execution succeeded
     */
    bool CompileFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            LogError("Failed to open script file: " + filename);
            return false;
        }
        
        std::string line;
        bool success = true;
        int lineNumber = 0;
        
        while (std::getline(file, line)) {
            lineNumber++;
            // Skip empty lines and comments
            if (line.empty() || line[0] == '#' || line[0] == ';') {
                continue;
            }
            
            if (!CompileAndExecute(line)) {
                LogError("Error on line " + std::to_string(lineNumber) + ": " + line);
                success = false;
            }
        }
        
        file.close();
        return success;
    }
    
    /**
     * Register a custom function that can be called from scripts
     * @param name Function name to use in scripts
     * @param function The C++ function to execute
     */
    void RegisterFunction(const std::string& name, ScriptFunction function) {
        functions_[name] = function;
    }
    
    /**
     * Set a variable value in the script environment
     * @param name Variable name
     * @param value Variable value
     */
    void SetVariable(const std::string& name, const std::string& value) {
        variables_[name] = value;
    }
    
    /**
     * Get a variable value from the script environment
     * @param name Variable name
     * @return Variable value, or empty string if not found
     */
    std::string GetVariable(const std::string& name) const {
        auto it = variables_.find(name);
        if (it != variables_.end()) {
            return it->second;
        }
        return "";
    }
    
    /**
     * Check if a variable exists
     * @param name Variable name
     * @return true if variable exists
     */
    bool HasVariable(const std::string& name) const {
        return variables_.find(name) != variables_.end();
    }
    
    /**
     * Get all registered functions
     * @return Map of function names
     */
    std::vector<std::string> GetRegisteredFunctions() const {
        std::vector<std::string> names;
        for (const auto& pair : functions_) {
            names.push_back(pair.first);
        }
        return names;
    }
    
    /**
     * Get all variables
     * @return Map of variable names and values
     */
    const std::map<std::string, std::string>& GetVariables() const {
        return variables_;
    }
    
    /**
     * Clear all variables
     */
    void ClearVariables() {
        variables_.clear();
    }
    
    /**
     * Enable or disable verbose logging
     * @param enabled true to enable verbose logging
     */
    void SetVerbose(bool enabled) {
        verbose_ = enabled;
    }
    
private:
    std::map<std::string, ScriptFunction> functions_;
    std::map<std::string, std::string> variables_;
    bool verbose_ = false;
    
    /**
     * Register built-in functions
     */
    void RegisterBuiltInFunctions() {
        // Built-in: set variable
        RegisterFunction("set", [this](const std::vector<std::string>& args) {
            if (args.size() >= 2) {
                std::string value = args[1];
                // Join remaining args as value
                for (size_t i = 2; i < args.size(); i++) {
                    value += " " + args[i];
                }
                SetVariable(args[0], value);
                if (verbose_) {
                    std::cout << "[ScriptCompiler] Set variable '" << args[0] 
                              << "' = '" << value << "'" << std::endl;
                }
            }
        });
        
        // Built-in: get variable
        RegisterFunction("get", [this](const std::vector<std::string>& args) {
            if (args.size() >= 1) {
                std::string value = GetVariable(args[0]);
                std::cout << args[0] << " = " << value << std::endl;
            }
        });
        
        // Built-in: print
        RegisterFunction("print", [](const std::vector<std::string>& args) {
            for (size_t i = 0; i < args.size(); i++) {
                std::cout << args[i];
                if (i < args.size() - 1) std::cout << " ";
            }
            std::cout << std::endl;
        });
        
        // Built-in: echo (same as print)
        RegisterFunction("echo", [](const std::vector<std::string>& args) {
            for (size_t i = 0; i < args.size(); i++) {
                std::cout << args[i];
                if (i < args.size() - 1) std::cout << " ";
            }
            std::cout << std::endl;
        });
    }
    
    /**
     * Tokenize a script line into words
     * @param script The script line
     * @return Vector of tokens
     */
    std::vector<std::string> Tokenize(const std::string& script) {
        std::vector<std::string> tokens;
        std::stringstream ss(script);
        std::string token;
        
        while (ss >> token) {
            // Handle quoted strings
            if (token[0] == '"') {
                std::string quotedString = token.substr(1);
                if (token.back() != '"') {
                    std::string part;
                    while (ss >> part) {
                        quotedString += " " + part;
                        if (part.back() == '"') {
                            quotedString = quotedString.substr(0, quotedString.length() - 1);
                            break;
                        }
                    }
                } else {
                    quotedString = quotedString.substr(0, quotedString.length() - 1);
                }
                tokens.push_back(quotedString);
            } else {
                tokens.push_back(token);
            }
        }
        
        return tokens;
    }
    
    /**
     * Execute tokenized script
     * @param tokens The tokenized script
     * @return true if execution succeeded
     */
    bool Execute(const std::vector<std::string>& tokens) {
        if (tokens.empty()) {
            return false;
        }
        
        std::string command = tokens[0];
        
        // Check if it's a registered function
        auto it = functions_.find(command);
        if (it != functions_.end()) {
            // Extract arguments (everything after the command)
            std::vector<std::string> args(tokens.begin() + 1, tokens.end());
            
            // Substitute variables in arguments
            for (auto& arg : args) {
                if (arg.length() > 1 && arg[0] == '$') {
                    std::string varName = arg.substr(1);
                    arg = GetVariable(varName);
                }
            }
            
            // Execute the function
            try {
                it->second(args);
                return true;
            } catch (const std::exception& e) {
                LogError("Exception executing function '" + command + "': " + e.what());
                return false;
            }
        } else {
            LogError("Unknown command: " + command);
            return false;
        }
    }
    
    /**
     * Log an error message
     * @param message The error message
     */
    void LogError(const std::string& message) {
        std::cerr << "[ScriptCompiler Error] " << message << std::endl;
    }
};

/**
 * ScriptCompilerManager - Singleton manager for the script compiler
 * Provides global access to the script compilation system
 */
class ScriptCompilerManager {
public:
    static ScriptCompilerManager& GetInstance() {
        static ScriptCompilerManager instance;
        return instance;
    }
    
    ScriptCompiler& GetCompiler() {
        return compiler_;
    }
    
    /**
     * Initialize the script compiler with engine bindings
     * This should be called during engine initialization
     */
    void Initialize() {
        if (initialized_) {
            return;
        }
        
        std::cout << "[ScriptCompiler] Initializing custom script compiler..." << std::endl;
        
        // Register engine-specific functions here
        // These can be extended by the game engine systems
        
        initialized_ = true;
        std::cout << "[ScriptCompiler] Script compiler initialized successfully!" << std::endl;
    }
    
    /**
     * Load and execute a script file
     * @param filename Path to the script file
     * @return true if successful
     */
    bool LoadScript(const std::string& filename) {
        return compiler_.CompileFile(filename);
    }
    
    /**
     * Execute a script command
     * @param command The script command
     * @return true if successful
     */
    bool ExecuteCommand(const std::string& command) {
        return compiler_.CompileAndExecute(command);
    }
    
private:
    ScriptCompiler compiler_;
    bool initialized_ = false;
    
    ScriptCompilerManager() = default;
    ScriptCompilerManager(const ScriptCompilerManager&) = delete;
    ScriptCompilerManager& operator=(const ScriptCompilerManager&) = delete;
};

#endif // SCRIPTCOMPILER_H
