using System;
using System.Windows;
using System.Windows.Input;
using System.Windows.Threading;

namespace GameEngine.Editor
{
    public partial class MainWindow : Window
    {
        private DispatcherTimer _updateTimer;
        private DateTime _lastFrameTime;
        private bool _isEditorMode = true;

        public bool IsEditorMode
        {
            get => _isEditorMode;
            set
            {
                _isEditorMode = value;
                EngineInterop.SetEditorMode(value);
                LogToConsole($"Editor mode: {(value ? "ON" : "OFF")}");
            }
        }

        public MainWindow()
        {
            InitializeComponent();
            DataContext = this;
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            try
            {
                // Initialize the engine
                IntPtr hwnd = EnginePanel.Handle;
                int width = (int)EnginePanel.Width;
                int height = (int)EnginePanel.Height;

                bool initialized = EngineInterop.InitializeEngine(hwnd, width, height);
                if (!initialized)
                {
                    LogToConsole("ERROR: Failed to initialize engine!");
                    MessageBox.Show("Failed to initialize the engine. Please check DirectX 11 support.", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }

                LogToConsole("Engine initialized successfully");
                LogToConsole("Welcome to Game Engine Editor - .NET 9 + C++20");
                LogToConsole("Press F1 for help");
                
                // Generate initial terrain
                EngineInterop.GenerateTerrain(12345);
                LogToConsole("Generated initial terrain");

                // Set initial editor mode
                EngineInterop.SetEditorMode(_isEditorMode);

                // Start update timer
                _updateTimer = new DispatcherTimer();
                _updateTimer.Interval = TimeSpan.FromMilliseconds(16); // ~60 FPS
                _updateTimer.Tick += UpdateTimer_Tick;
                _updateTimer.Start();
                _lastFrameTime = DateTime.Now;

                StatusText.Text = "Engine running";
            }
            catch (Exception ex)
            {
                LogToConsole($"ERROR: {ex.Message}");
                MessageBox.Show($"Error initializing engine: {ex.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void UpdateTimer_Tick(object? sender, EventArgs e)
        {
            try
            {
                // Calculate delta time
                DateTime now = DateTime.Now;
                float deltaTime = (float)(now - _lastFrameTime).TotalSeconds;
                _lastFrameTime = now;

                // Update and render engine
                EngineInterop.UpdateEngine(deltaTime);
                EngineInterop.RenderEngine();

                // Update UI
                FpsText.Text = ((int)(1.0f / deltaTime)).ToString();
                
                // Update camera position display
                EngineInterop.GetCameraPosition(out float x, out float y, out float z);
                CameraPositionText.Text = $"({x:F1}, {y:F1}, {z:F1})";

                // Handle keyboard input
                HandleKeyboardInput(deltaTime);
            }
            catch (Exception ex)
            {
                LogToConsole($"ERROR in update loop: {ex.Message}");
            }
        }

        private void HandleKeyboardInput(float deltaTime)
        {
            float speed = 10.0f * deltaTime;

            if (Keyboard.IsKeyDown(Key.W))
                EngineInterop.MoveCameraForward(speed);
            if (Keyboard.IsKeyDown(Key.S))
                EngineInterop.MoveCameraForward(-speed);
            if (Keyboard.IsKeyDown(Key.A))
                EngineInterop.MoveCameraRight(-speed);
            if (Keyboard.IsKeyDown(Key.D))
                EngineInterop.MoveCameraRight(speed);
            if (Keyboard.IsKeyDown(Key.Space))
                EngineInterop.MoveCameraUp(speed);
            if (Keyboard.IsKeyDown(Key.LeftShift))
                EngineInterop.MoveCameraUp(-speed);
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            try
            {
                _updateTimer?.Stop();
                EngineInterop.ShutdownEngine();
                LogToConsole("Engine shut down successfully");
            }
            catch (Exception ex)
            {
                LogToConsole($"ERROR during shutdown: {ex.Message}");
            }
        }

        private void LogToConsole(string message)
        {
            Dispatcher.Invoke(() =>
            {
                ConsoleOutput.AppendText($"[{DateTime.Now:HH:mm:ss}] {message}\n");
                ConsoleOutput.ScrollToEnd();
            });
        }

        // Menu Event Handlers
        private void NewScene_Click(object sender, RoutedEventArgs e)
        {
            LogToConsole("New scene created");
            MessageBox.Show("New scene functionality not yet implemented", "Info", MessageBoxButton.OK, MessageBoxImage.Information);
        }

        private void OpenScene_Click(object sender, RoutedEventArgs e)
        {
            LogToConsole("Open scene requested");
            MessageBox.Show("Open scene functionality not yet implemented", "Info", MessageBoxButton.OK, MessageBoxImage.Information);
        }

        private void SaveScene_Click(object sender, RoutedEventArgs e)
        {
            LogToConsole("Scene saved");
            MessageBox.Show("Save scene functionality not yet implemented", "Info", MessageBoxButton.OK, MessageBoxImage.Information);
        }

        private void Exit_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }

        private void ToggleEditorMode_Click(object sender, RoutedEventArgs e)
        {
            IsEditorMode = !IsEditorMode;
        }

        private void GenerateTerrain_Click(object sender, RoutedEventArgs e)
        {
            Random rand = new Random();
            int seed = rand.Next();
            EngineInterop.GenerateTerrain(seed);
            LogToConsole($"Generated new terrain with seed: {seed}");
        }

        private void ToggleProperties_Click(object sender, RoutedEventArgs e)
        {
            LogToConsole("Toggle properties panel");
        }

        private void ToggleHierarchy_Click(object sender, RoutedEventArgs e)
        {
            LogToConsole("Toggle hierarchy panel");
        }

        private void ToggleConsole_Click(object sender, RoutedEventArgs e)
        {
            LogToConsole("Toggle console panel");
        }

        private void About_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show(
                "Game Engine Editor\n" +
                ".NET 9 + C++20 Hybrid Architecture\n\n" +
                "Features:\n" +
                "- Voxel-based terrain\n" +
                "- DirectX 11 rendering\n" +
                "- Dockable UI with AvalonDock\n" +
                "- Real-time editor\n\n" +
                "Controls:\n" +
                "WASD - Move camera\n" +
                "Space/Shift - Up/Down\n" +
                "Mouse - Look around",
                "About",
                MessageBoxButton.OK,
                MessageBoxImage.Information
            );
        }

        private void Play_Click(object sender, RoutedEventArgs e)
        {
            IsEditorMode = false;
            LogToConsole("Game mode activated");
        }

        private void Pause_Click(object sender, RoutedEventArgs e)
        {
            IsEditorMode = true;
            LogToConsole("Game paused - Editor mode");
        }

        private void PlaceBlock_Click(object sender, RoutedEventArgs e)
        {
            // Example: Place block at camera position
            EngineInterop.GetCameraPosition(out float x, out float y, out float z);
            EngineInterop.SetVoxel((int)x, (int)y, (int)z, 1); // Grass block
            LogToConsole($"Placed block at ({(int)x}, {(int)y}, {(int)z})");
        }

        private void RemoveBlock_Click(object sender, RoutedEventArgs e)
        {
            // Example: Remove block at camera position
            EngineInterop.GetCameraPosition(out float x, out float y, out float z);
            EngineInterop.SetVoxel((int)x, (int)y, (int)z, 0); // Air
            LogToConsole($"Removed block at ({(int)x}, {(int)y}, {(int)z})");
        }

        private void ConsoleInput_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                string command = ConsoleInput.Text.Trim();
                if (!string.IsNullOrEmpty(command))
                {
                    LogToConsole($"> {command}");
                    ExecuteCommand(command);
                    ConsoleInput.Clear();
                }
            }
        }

        private void ExecuteCommand(string command)
        {
            string[] parts = command.Split(' ', StringSplitOptions.RemoveEmptyEntries);
            if (parts.Length == 0) return;

            switch (parts[0].ToLower())
            {
                case "help":
                    LogToConsole("Available commands:");
                    LogToConsole("  help - Show this help");
                    LogToConsole("  clear - Clear console");
                    LogToConsole("  terrain <seed> - Generate new terrain");
                    LogToConsole("  setcam <x> <y> <z> - Set camera position");
                    LogToConsole("  editor - Toggle editor mode");
                    break;
                case "clear":
                    ConsoleOutput.Clear();
                    break;
                case "terrain":
                    if (parts.Length > 1 && int.TryParse(parts[1], out int seed))
                    {
                        EngineInterop.GenerateTerrain(seed);
                        LogToConsole($"Generated terrain with seed: {seed}");
                    }
                    else
                    {
                        LogToConsole("Usage: terrain <seed>");
                    }
                    break;
                case "setcam":
                    if (parts.Length > 3 &&
                        float.TryParse(parts[1], out float x) &&
                        float.TryParse(parts[2], out float y) &&
                        float.TryParse(parts[3], out float z))
                    {
                        EngineInterop.SetCameraPosition(x, y, z);
                        LogToConsole($"Camera position set to ({x}, {y}, {z})");
                    }
                    else
                    {
                        LogToConsole("Usage: setcam <x> <y> <z>");
                    }
                    break;
                case "editor":
                    IsEditorMode = !IsEditorMode;
                    break;
                default:
                    LogToConsole($"Unknown command: {parts[0]}");
                    break;
            }
        }
    }
}
