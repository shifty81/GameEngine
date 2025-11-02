# itch.io Integration Guide

## Overview

This game engine includes seamless integration with the itch.io platform, allowing your game to:

- **Authenticate users** automatically when launched from the itch.io app
- **Verify purchases** to enable premium features for legitimate buyers
- **Access user profiles** to personalize the gaming experience
- **Support cloud features** for save games and achievements (with custom backend)

## Quick Start

### 1. Basic Integration

Include the ItchIO header in your game code:

```cpp
#include "ItchIO.h"

int main() {
    // Create ItchIO API instance
    ItchIO::API itchio;
    
    // Check if running from itch.io app
    if (itchio.IsAvailable()) {
        std::cout << "Running from itch.io app!" << std::endl;
        
        // Initialize and verify connection
        if (itchio.Initialize()) {
            // Get user profile
            auto profile = itchio.GetUserProfile();
            if (profile) {
                std::cout << "Welcome, " << profile->username << "!" << std::endl;
            }
        }
    } else {
        std::cout << "Running standalone - itch.io features disabled" << std::endl;
    }
    
    // Your game code here...
    
    return 0;
}
```

### 2. Verifying Purchase

To check if a user legitimately owns your game:

```cpp
ItchIO::API itchio;

if (itchio.IsAvailable() && itchio.Initialize()) {
    // Replace "12345" with your actual game ID from itch.io
    if (itchio.VerifyPurchase("12345")) {
        std::cout << "Purchase verified - enabling premium features" << std::endl;
        enablePremiumFeatures();
    } else {
        std::cout << "Free version detected" << std::endl;
        showUpgradePrompt();
    }
}
```

### 3. Personalizing User Experience

Use profile information to customize the game:

```cpp
ItchIO::API itchio;

if (itchio.IsAvailable() && itchio.Initialize()) {
    auto profile = itchio.GetUserProfile();
    if (profile) {
        // Display personalized welcome message
        std::string welcome = "Welcome back, " + profile->display_name + "!";
        ui->ShowMessage(welcome);
        
        // Load user-specific settings from cloud
        loadCloudSaveData(profile->id);
        
        // Check user type for special features
        if (profile->developer) {
            enableDeveloperMode();
        }
    }
}
```

## How It Works

### Environment Variable Detection

When your game is launched from the itch.io app, the app automatically sets the `ITCHIO_API_KEY` environment variable. This is a JWT (JSON Web Token) that authenticates your game with the itch.io API.

The ItchIO class automatically:
1. Checks for the `ITCHIO_API_KEY` environment variable
2. Uses this key to authenticate API requests
3. Validates the key by fetching the user profile

### API Communication

The integration uses the itch.io REST API:

- **Base URL**: `https://itch.io/api/1`
- **Authentication**: Bearer token (JWT from environment)
- **Endpoints used**:
  - `/jwt/me` - Get current user information
  - `/games/{game_id}/purchases` - Verify game ownership

## Publishing Your Game on itch.io

### 1. Create App Manifest (Optional)

For advanced integration, create a `.itch.toml` file in your game's root directory:

```toml
[[actions]]
name = "play"
path = "GameEngine.exe"  # Or your executable name
scope = "profile:me"

[[actions]]
name = "play-windowed"
path = "GameEngine.exe"
args = ["--windowed"]
scope = "profile:me"
```

This tells the itch.io app:
- Which executable to launch
- What API scopes your game needs
- Custom launch options

### 2. Upload Your Game

1. Go to https://itch.io/game/new
2. Fill in game details
3. Upload your build (Windows/Linux/Mac)
4. Enable "itch.io app integration" in settings
5. Publish your game

### 3. Testing Integration

To test itch.io integration during development:

**Option A: Set environment variable manually**
```bash
# Windows
set ITCHIO_API_KEY=your_test_token_here
GameEngine.exe

# Linux/Mac
export ITCHIO_API_KEY=your_test_token_here
./GameEngine
```

**Option B: Install via itch.io app**
1. Upload a test build to itch.io (can be private)
2. Install through the itch.io app
3. Launch from the app - integration will work automatically

## Advanced Features

### Custom HTTP Client

If you want to use libcurl or another HTTP library:

```cpp
#include "ItchIO.h"
#include <curl/curl.h>

class CurlHTTPClient : public ItchIO::HTTPClient {
public:
    HTTPResponse Get(const std::string& url, 
                    const std::map<std::string, std::string>& headers) override {
        // Implement using libcurl
        CURL* curl = curl_easy_init();
        // ... setup and perform request
        // ... return response
    }
};

// Use custom HTTP client
auto http_client = std::make_unique<CurlHTTPClient>();
ItchIO::API itchio(std::move(http_client));
```

### Cloud Save Integration

The ItchIO integration provides user authentication, which you can use with your own backend:

```cpp
ItchIO::API itchio;

if (itchio.Initialize()) {
    auto profile = itchio.GetUserProfile();
    
    // Use user ID to sync with your cloud backend
    std::string save_url = "https://your-backend.com/saves/" + 
                          std::to_string(profile->id);
    
    // Upload save data
    uploadSaveData(save_url, itchio.GetAPIKey(), saveGameData);
    
    // Download save data
    auto cloudSave = downloadSaveData(save_url, itchio.GetAPIKey());
}
```

### Achievement System

While itch.io doesn't have native achievements, you can implement your own:

```cpp
class AchievementSystem {
private:
    ItchIO& itchio_;
    std::string backend_url_;
    
public:
    AchievementSystem(ItchIO& itchio, const std::string& backend_url)
        : itchio_(itchio), backend_url_(backend_url) {}
    
    bool UnlockAchievement(const std::string& achievement_id) {
        auto profile = itchio_.GetUserProfile();
        if (!profile) return false;
        
        // Send to your backend
        std::string url = backend_url_ + "/achievements/" + 
                         std::to_string(profile->id);
        
        // Use itchio API key for authentication
        return postAchievement(url, itchio_.GetAPIKey(), achievement_id);
    }
    
    std::vector<std::string> GetUnlockedAchievements() {
        auto profile = itchio_.GetUserProfile();
        if (!profile) return {};
        
        std::string url = backend_url_ + "/achievements/" + 
                         std::to_string(profile->id);
        
        return fetchAchievements(url, itchio_.GetAPIKey());
    }
};

// Usage
ItchIO::API itchio;
if (itchio.Initialize()) {
    AchievementSystem achievements(itchio, "https://your-backend.com");
    
    // When player completes a milestone
    if (playerScore > 1000) {
        achievements.UnlockAchievement("first_1000_points");
    }
}
```

## API Reference

### ItchIO::API Class

#### Constructor
```cpp
ItchIO::API();  // Auto-detects API key from environment
ItchIO::API(const std::string& api_key);  // Use explicit API key
```

#### Methods

##### `bool IsAvailable()`
Check if itch.io integration is available (API key exists).

##### `bool Initialize()`
Initialize and verify the API connection. Returns true on success.

##### `std::shared_ptr<UserProfile> GetUserProfile()`
Get the current user's profile. Returns nullptr on failure.

##### `bool VerifyPurchase(const std::string& game_id)`
Verify if the user owns/purchased the game. Returns true if verified.

##### `const std::string& GetAPIKey()`
Get the raw API key being used.

##### `bool IsInitialized()`
Check if API has been successfully initialized.

### UserProfile Structure

```cpp
struct UserProfile {
    int id;                    // Unique user ID
    std::string username;      // Username (e.g., "johndoe")
    std::string display_name;  // Display name (e.g., "John Doe")
    std::string cover_url;     // Profile cover image URL
    std::string url;           // Profile URL (e.g., "https://johndoe.itch.io")
    bool gamer;               // Is a gamer account
    bool developer;           // Is a developer account
    bool press_user;          // Is a press/media account
};
```

## Best Practices

### 1. Graceful Degradation

Always check if itch.io is available and handle both cases:

```cpp
ItchIO::API itchio;

if (itchio.IsAvailable() && itchio.Initialize()) {
    // Online/itch.io features
    enableCloudSaves();
    enableAchievements();
} else {
    // Offline/standalone features
    enableLocalSavesOnly();
    disableAchievements();
}
```

### 2. Don't Block Game Start

Initialize itch.io asynchronously so it doesn't delay game startup:

```cpp
void initializeItchIO() {
    std::thread([]{
        ItchIO::API itchio;
        if (itchio.Initialize()) {
            // Enable features after initialization
            Game::instance->enableItchIOFeatures();
        }
    }).detach();
}
```

### 3. Cache User Data

The ItchIO class automatically caches the user profile, but you should also cache relevant data:

```cpp
class GameState {
    std::shared_ptr<ItchIO::UserProfile> userProfile;
    
    void loadUser() {
        ItchIO::API itchio;
        if (itchio.Initialize()) {
            userProfile = itchio.GetUserProfile();
            // Don't call GetUserProfile() repeatedly
        }
    }
};
```

### 4. Error Handling

Always check return values and handle errors:

```cpp
ItchIO::API itchio;

if (!itchio.IsAvailable()) {
    std::cout << "Note: Running without itch.io integration" << std::endl;
    // Continue with game...
    return;
}

if (!itchio.Initialize()) {
    std::cerr << "Warning: Failed to initialize itch.io API" << std::endl;
    // Continue with game, but disable online features
    return;
}

auto profile = itchio.GetUserProfile();
if (!profile) {
    std::cerr << "Warning: Could not fetch user profile" << std::endl;
    // Continue with game...
    return;
}
```

## Finding Your Game ID

To use `VerifyPurchase()`, you need your itch.io game ID:

1. Go to your game's dashboard on itch.io
2. Look at the URL: `https://itch.io/game/dashboard/12345`
3. The number `12345` is your game ID
4. Use it in your code: `itchio.VerifyPurchase("12345")`

## Limitations and Notes

### Current Implementation

- **HTTP Client**: The default HTTP client is a basic implementation. For production use, integrate with libcurl (see "Custom HTTP Client" section)
- **JSON Parsing**: Uses a simple JSON parser. For complex data, consider using nlohmann/json or rapidjson
- **Achievements**: itch.io doesn't have native achievements. You need to implement your own backend

### Security

- **API Key Storage**: Never hardcode API keys or commit them to version control
- **HTTPS Only**: Always use HTTPS endpoints for API requests
- **Token Validation**: The API key is a JWT that expires. Handle expiration gracefully
- **Purchase Verification**: Verify purchases server-side if possible to prevent tampering

### Platform Support

- **Windows**: Fully supported
- **Linux**: Fully supported
- **Mac**: Fully supported
- **Web**: Not applicable (use itch.io web API directly in JavaScript)

## Troubleshooting

### "No API key available" message

**Problem**: Game shows "Game not launched from itch.io app"

**Solutions**:
- Make sure you're launching from the itch.io app, not directly
- Check that your `.itch.toml` manifest includes `scope = "profile:me"`
- For testing, manually set the environment variable

### API requests fail

**Problem**: Initialize() returns false

**Solutions**:
- Check your internet connection
- Verify the API key is valid (check console output)
- Make sure you're using HTTPS endpoints
- Consider implementing a custom HTTP client with proper error handling

### Purchase verification not working

**Problem**: VerifyPurchase() always returns false

**Solutions**:
- Verify you're using the correct game ID
- Ensure the user actually owns the game on itch.io
- Check that your game has purchase verification enabled in itch.io settings
- Test with a different user account

## Example: Complete Integration

Here's a complete example showing all features:

```cpp
#include "ItchIO.h"
#include <iostream>
#include <memory>

class Game {
private:
    std::unique_ptr<ItchIO> itchio_;
    std::shared_ptr<ItchIO::UserProfile> player_profile_;
    bool premium_features_ = false;
    
public:
    void Initialize() {
        // Initialize itch.io integration
        itchio_ = std::make_unique<ItchIO>();
        
        if (itchio_->IsAvailable()) {
            std::cout << "[Game] itch.io integration available" << std::endl;
            
            if (itchio_->Initialize()) {
                std::cout << "[Game] itch.io initialized successfully" << std::endl;
                
                // Get player profile
                player_profile_ = itchio_->GetUserProfile();
                if (player_profile_) {
                    std::cout << "[Game] Welcome, " << player_profile_->display_name << "!" << std::endl;
                    
                    // Check purchase status
                    if (itchio_->VerifyPurchase("your-game-id")) {
                        std::cout << "[Game] Premium version verified" << std::endl;
                        premium_features_ = true;
                    } else {
                        std::cout << "[Game] Free version - upgrade for more features" << std::endl;
                    }
                }
            } else {
                std::cout << "[Game] Failed to initialize itch.io" << std::endl;
            }
        } else {
            std::cout << "[Game] Running standalone (no itch.io integration)" << std::endl;
        }
    }
    
    void ShowMainMenu() {
        std::cout << "\n=== Main Menu ===" << std::endl;
        std::cout << "1. Start Game" << std::endl;
        std::cout << "2. Options" << std::endl;
        
        if (player_profile_) {
            std::cout << "3. View Profile (" << player_profile_->username << ")" << std::endl;
        }
        
        if (premium_features_) {
            std::cout << "4. Premium Features" << std::endl;
        } else {
            std::cout << "4. Upgrade to Premium" << std::endl;
        }
        
        std::cout << "5. Exit" << std::endl;
    }
    
    bool IsPremium() const { return premium_features_; }
    const ItchIO::UserProfile* GetPlayerProfile() const { return player_profile_.get(); }
};

int main() {
    Game game;
    game.Initialize();
    game.ShowMainMenu();
    
    // Continue with game loop...
    
    return 0;
}
```

## Further Resources

- [itch.io API Documentation](https://itch.io/docs/api/overview)
- [itch.io App Integration Guide](https://itch.io/docs/itch/integrating/)
- [Server-side API Reference](https://itch.io/docs/api/serverside)
- [OAuth Applications](https://itch.io/docs/api/oauth)

## Support

For issues with this integration:
- Check the console output for error messages
- Review this documentation
- Test with a simple standalone example first

For issues with the itch.io platform:
- Visit https://itch.io/docs
- Contact itch.io support
- Check the itch.io developer forums
