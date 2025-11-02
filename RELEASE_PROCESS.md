# Release Process Guide

This document describes how to create and publish new releases with pre-built binaries.

## Automated Release Process

The repository is configured with GitHub Actions to automatically build and publish binaries when you create a release tag.

### Creating a New Release

#### Option 1: Using Git Tags (Recommended)

1. **Update version numbers** in your code if needed
2. **Commit all changes**
3. **Create and push a version tag:**

```bash
# Create a tag (e.g., v1.0.0, v1.1.0, v2.0.0)
git tag -a v1.0.0 -m "Release version 1.0.0"

# Push the tag to GitHub
git push origin v1.0.0
```

4. **GitHub Actions will automatically:**
   - Build Windows binaries (Visual Studio 2022, Release mode)
   - Build Linux binaries (GCC, Release mode)
   - Create a GitHub Release
   - Upload the binaries to the release

5. **Monitor the build:**
   - Go to: https://github.com/shifty81/GameEngine/actions
   - Watch the "Build and Release" workflow
   - Build takes approximately 15-20 minutes

6. **Verify the release:**
   - Go to: https://github.com/shifty81/GameEngine/releases
   - Check that both Windows and Linux binaries are attached

#### Option 2: Manual Trigger

You can also manually trigger a build without creating a tag:

1. Go to: https://github.com/shifty81/GameEngine/actions
2. Click "Build and Release" workflow
3. Click "Run workflow" button
4. Enter version number (e.g., "1.0.0")
5. Click "Run workflow"

**Note:** This creates artifacts but does NOT create a GitHub Release. You'll need to download artifacts and create the release manually.

---

## What Gets Built

### Windows Build (`GameEngine-Windows-x64.zip`)

**Contents:**
- `GameEngine.exe` - Compiled executable (Release, x64)
- `assets/` - All game assets
- `README.md` - Main documentation
- `QUICKSTART.md` - Quick start guide
- `LICENSE` - License file
- `run.bat` - Convenient run script
- `README_BINARIES.txt` - Instructions for binary users

**Build Configuration:**
- Compiler: MSVC (Visual Studio 2022)
- Architecture: x64
- Configuration: Release (optimized)
- Platform: Windows 10/11

### Linux Build (`GameEngine-Linux-x64.tar.gz`)

**Contents:**
- `GameEngine` - Compiled executable (Release, x64)
- `assets/` - All game assets
- `README.md` - Main documentation
- `QUICKSTART.md` - Quick start guide
- `LICENSE` - License file
- `run.sh` - Convenient run script (executable)
- `README_BINARIES.txt` - Instructions for binary users

**Build Configuration:**
- Compiler: GCC (latest on Ubuntu)
- Architecture: x64
- Configuration: Release (optimized)
- Platform: Ubuntu 20.04+ compatible

---

## Version Numbering

Follow semantic versioning (SemVer): `vMAJOR.MINOR.PATCH`

**Examples:**
- `v1.0.0` - First stable release
- `v1.1.0` - New features added
- `v1.1.1` - Bug fixes
- `v2.0.0` - Breaking changes

**When to increment:**
- **MAJOR**: Breaking changes, major rewrites
- **MINOR**: New features, backwards compatible
- **PATCH**: Bug fixes, small improvements

---

## Pre-release and Beta Versions

For pre-release versions, use tags like:
- `v1.0.0-alpha.1` - Alpha version
- `v1.0.0-beta.1` - Beta version
- `v1.0.0-rc.1` - Release candidate

Mark these as "pre-release" when creating the GitHub Release.

---

## Build Artifacts Retention

- **Artifacts** (from manual/non-tag builds): Kept for **90 days**
- **Releases** (from tags): Kept **indefinitely**

---

## Testing Before Release

Before creating a release tag, test the builds:

1. **Manual workflow trigger** to create test artifacts
2. **Download and test** both Windows and Linux builds
3. **Verify:**
   - Executable runs correctly
   - Assets are included
   - Documentation is present
   - No missing DLLs or dependencies
4. **Fix any issues** and test again
5. **Create release tag** once verified

---

## Troubleshooting Build Failures

### Windows Build Fails

**Check:**
- CMake configuration errors
- Missing submodules
- Compilation errors in code
- Path issues in packaging step

**View logs:**
- Go to Actions tab → Failed workflow → Windows build job
- Review "Configure CMake" and "Build Release" steps

### Linux Build Fails

**Check:**
- Missing dependencies in apt-get install step
- Compilation errors (different from MSVC)
- CMake configuration issues

**View logs:**
- Go to Actions tab → Failed workflow → Linux build job
- Review "Configure CMake" and "Build Release" steps

### Release Creation Fails

**Check:**
- GitHub token permissions
- Both builds completed successfully
- Tag format is correct (starts with 'v')

---

## Manual Release Creation (Fallback)

If automated release fails, create manually:

1. **Build locally** or download artifacts from Actions
2. **Create a new release** on GitHub:
   - Go to: https://github.com/shifty81/GameEngine/releases
   - Click "Draft a new release"
   - Choose tag or create new tag
   - Fill in release notes
   - Upload `GameEngine-Windows-x64.zip`
   - Upload `GameEngine-Linux-x64.tar.gz`
   - Publish release

---

## Release Notes Template

Use this template for release descriptions:

```markdown
## GameEngine v1.0.0

### New Features
- Feature 1 description
- Feature 2 description

### Improvements
- Improvement 1
- Improvement 2

### Bug Fixes
- Fixed issue #123
- Fixed crash when...

### Downloads
- **Windows**: GameEngine-Windows-x64.zip
- **Linux**: GameEngine-Linux-x64.tar.gz

### System Requirements
- Windows 10/11 (64-bit) or Linux (64-bit)
- OpenGL 3.3+ compatible GPU
- 2GB RAM minimum

### Installation
See [DOWNLOAD_BINARIES.md](DOWNLOAD_BINARIES.md) for detailed instructions.

### For Developers
Build from source: [README.md](README.md)
```

---

## Updating the Workflow

The build workflow is defined in `.github/workflows/build-release.yml`.

**To modify:**
1. Edit the YAML file
2. Test changes by triggering manually first
3. Commit and push
4. Verify next release works correctly

**Common modifications:**
- Update CMake version
- Change compiler settings
- Modify packaging steps
- Adjust artifact retention
- Add macOS builds

---

## Best Practices

1. **Test before releasing:** Always test artifacts before tagging
2. **Write clear release notes:** Help users understand what changed
3. **Follow semantic versioning:** Makes version numbers meaningful
4. **Keep builds reproducible:** Don't modify workflow between builds
5. **Archive old releases:** Keep at least last 3 major versions

---

## Quick Reference

```bash
# Create and push a release tag
git tag -a v1.0.0 -m "Release version 1.0.0"
git push origin v1.0.0

# Delete a tag if needed (before builds complete)
git tag -d v1.0.0
git push origin :refs/tags/v1.0.0

# List all tags
git tag -l

# View workflow status
# Visit: https://github.com/shifty81/GameEngine/actions
```

---

## Support

For issues with the build workflow:
1. Check Actions tab for error details
2. Review workflow file syntax
3. Consult GitHub Actions documentation
4. Open an issue if workflow is broken
