# Testing Binaries

This document explains how to access and test the automatically built binaries from the CI/CD pipeline.

## Overview

The GameEngine repository uses GitHub Actions to automatically build and test binaries on every push and pull request. This ensures that the code always compiles successfully on both Windows and Linux platforms.

## Build Test Workflow

### What Gets Tested

Every commit triggers the **Build Test** workflow, which:
1. Checks out the code with all submodules
2. Sets up the build environment (CMake, compilers, dependencies)
3. Builds the GameEngine in Release configuration
4. Verifies the executable was created successfully
5. Uploads the binary as an artifact for testing

### Platforms Tested

- **Windows**: Visual Studio 2022 with MSVC compiler
- **Linux**: Ubuntu 24.04 LTS with GCC 13.3 compiler

## Accessing Test Binaries

### From GitHub Actions Tab

1. Navigate to the repository on GitHub
2. Click the **Actions** tab at the top
3. Click on the **Build Test** workflow in the left sidebar
4. Select a specific workflow run from the list
5. Scroll down to the **Artifacts** section
6. Download the artifacts:
   - `GameEngine-Windows-Test-<commit-sha>` - Windows executable
   - `GameEngine-Linux-Test-<commit-sha>` - Linux executable

### From Pull Requests

1. Open any Pull Request
2. Look for the build status check at the bottom
3. Click **Details** next to the Build Test check
4. This takes you to the workflow run where you can download artifacts

## Retention Policy

- Test artifacts are retained for **7 days**
- This is shorter than release binaries to save storage
- Download them soon after builds complete if you need them

## Testing the Binaries

### Windows

1. Download the `GameEngine-Windows-Test-<commit-sha>` artifact
2. Extract the ZIP file
3. Ensure you have [Visual C++ Redistributable 2022](https://aka.ms/vs/17/release/vc_redist.x64.exe) installed
4. Run `GameEngine.exe`

**Note**: Test binaries don't include assets, so you'll need to copy the `assets/` folder from the repository if required.

### Linux

1. Download the `GameEngine-Linux-Test-<commit-sha>` artifact
2. Extract the archive
3. Make it executable: `chmod +x GameEngine`
4. Install dependencies:
   ```bash
   sudo apt-get install libgl1-mesa-glx libglu1-mesa libx11-6 libxrandr2 libxinerama1 libxcursor1 libxi6
   ```
5. Run: `./GameEngine`

**Note**: Test binaries don't include assets, so you'll need to copy the `assets/` folder from the repository if required.

## Troubleshooting

### "Artifacts are expired"

Test artifacts are only kept for 7 days. If you need the binary, either:
- Trigger a new build by pushing a commit
- Use the manual trigger option in GitHub Actions
- Download pre-built release binaries from the [Releases page](https://github.com/shifty81/GameEngine/releases)

### Build Failed

If the Build Test workflow fails:
1. Click on the failed job to see the logs
2. Look for error messages in the build steps
3. Common issues:
   - Missing submodules - check if submodules initialized correctly
   - Compilation errors - review the compiler output
   - Missing dependencies - check the setup steps

### Can't Download Artifacts

- You must be logged into GitHub to download artifacts
- Artifacts are only available on the Actions tab, not from the commit page directly

## Release Binaries vs Test Binaries

### Test Binaries (Build Test Workflow)
- ✅ Built on every push/PR
- ✅ Fast feedback on code changes
- ✅ Includes basic verification
- ❌ Only includes the executable
- ❌ 7-day retention
- ❌ Not production-ready packages

### Release Binaries (Build and Release Workflow)
- ✅ Built on version tags
- ✅ Includes all assets and documentation
- ✅ Production-ready packages
- ✅ Permanent retention
- ✅ Published to GitHub Releases
- 🔗 See [DOWNLOAD_BINARIES.md](DOWNLOAD_BINARIES.md)

## For Contributors

### Testing Your Changes

Before submitting a Pull Request:

1. Push your changes to your branch
2. Wait for the Build Test workflow to complete
3. Verify both Windows and Linux builds pass
4. Download and test the artifacts if needed
5. Fix any build issues before requesting review

### Build Status Badge

The repository README includes a build status badge that shows the current build status. A green badge means all tests are passing.

## Manual Workflow Trigger

You can manually trigger a test build:

1. Go to the **Actions** tab
2. Select **Build Test** workflow
3. Click **Run workflow** button
4. Select the branch to build
5. Click **Run workflow**

This is useful for:
- Testing a specific branch without pushing new commits
- Re-running tests after transient failures
- Getting fresh artifacts when old ones have expired

## Related Documentation

- [README.md](README.md) - Main documentation and build instructions
- [DOWNLOAD_BINARIES.md](DOWNLOAD_BINARIES.md) - Downloading official release binaries
- [TROUBLESHOOTING.md](TROUBLESHOOTING.md) - Build troubleshooting guide
- [RELEASE_PROCESS.md](RELEASE_PROCESS.md) - How to create official releases

## Questions?

If you have questions about the CI/CD pipeline or testing binaries, please:
- Check the [TROUBLESHOOTING.md](TROUBLESHOOTING.md) guide
- Review the workflow logs in the Actions tab
- Open an issue on GitHub with the `question` label
