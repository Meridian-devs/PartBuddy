# Contributing to Projects
Thank you for your interest in contributing! This document provides guidelines and information for contributors.

## Ways to Contribute
- 🐛 Bug Reports: Help us identify and fix issues
- 💡 Feature Requests: Suggest new features or improvements
- 📖 Documentation: Improve our docs, examples, and guides
- 🔧 Code Contributions: Implement features, fix bugs, or improve performance
- 🧪 Testing: Write tests or help with quality assurance

## Getting Started
### Development Environment Setup
  1. **Fork the repository** on GitHub

  2. **Clone your fork** locally:

  ```

  git clone https://github.com/YOUR_USERNAME/REPO_NAME.git

  cd REPO_NAME

  ```
Refer to repository specific documentation on required dependencies for setting up running code.

### Verifying setup is complete
  1. Run the build script using CMake to build the object file. If the project is not C/C++ based, refer to repo documentation for building.
     - Most projects however should be C/C++ based.

  ```

  cmake -S . -B build -G Ninja

  cmake --build build

  ```
  2.  Run the object file, REPO_NAME.exe
  3. If successful, the program should launch, for terminal tools, test it by typing the 'help' command. For standalone applications, refer to the repo documentation for accessing the help feature.
  4. Play around with the tool and its features to get a feel for how it works.

## Bug Fixes and Feature Requests:

### Communication
- GitHub Issues: Bug reports and feature requests
- GitHub Discussions: General questions and community discussions
- Email: Security issues and sensitive matters

### Getting Help
- Documentation: Check our docs first
- Search existing issues: Your question might already be answered
- Ask in discussions: For general questions

### 🏷️ Release Process
Versioning
We follow Semantic Versioning:
- MAJOR: Breaking changes
- MINOR: New features (backward compatible)
- PATCH: Bug fixes (backward compatible)

## 📞 Questions?
If you have questions about contributing:
- Check the FAQ
- Search existing discussions
- Create a new discussion

Additionally, feel free to contact the dev team: contact.meridiandev@gmail.com
Thank you for your contributions! 🙏
