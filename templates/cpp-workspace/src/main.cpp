#include <iostream>
#include <string>
#include <vector>
#include <memory>

/**
 * @brief Example class demonstrating modern C++ features
 */
class Application {
public:
    Application(std::string name) : name_(std::move(name)) {
        std::cout << "Application created: " << name_ << std::endl;
    }

    ~Application() {
        std::cout << "Application destroyed: " << name_ << std::endl;
    }

    // Delete copy constructor and assignment operator (rule of five)
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    // Move constructor and assignment operator
    Application(Application&&) noexcept = default;
    Application& operator=(Application&&) noexcept = default;

    void run() {
        std::cout << "Running " << name_ << "..." << std::endl;
        
        // Example: Using modern C++20 features
        std::vector<int> numbers = {1, 2, 3, 4, 5};
        
        std::cout << "Numbers: ";
        for (const auto& num : numbers) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }

    [[nodiscard]] const std::string& getName() const {
        return name_;
    }

private:
    std::string name_;
};

int main(int argc, char* argv[]) {
    std::cout << "C++ Workspace Template" << std::endl;
    std::cout << "=======================" << std::endl;

    // Create application using smart pointer
    auto app = std::make_unique<Application>("CPP-Workspace");
    
    // Run the application
    app->run();

    std::cout << "\nBuild Configuration:" << std::endl;
    #ifdef NDEBUG
        std::cout << "  Mode: Release" << std::endl;
    #else
        std::cout << "  Mode: Debug" << std::endl;
    #endif
    
    std::cout << "  C++ Standard: " << __cplusplus << std::endl;
    
    std::cout << "\nAvailable Libraries:" << std::endl;
    
    #ifdef OPENGL_FOUND
        std::cout << "  ✓ OpenGL" << std::endl;
    #endif
    
    #ifdef VULKAN_FOUND
        std::cout << "  ✓ Vulkan" << std::endl;
    #endif
    
    #ifdef SDL2_FOUND
        std::cout << "  ✓ SDL2" << std::endl;
    #endif
    
    #ifdef QT_VERSION
        std::cout << "  ✓ Qt6" << std::endl;
    #endif

    std::cout << "\n✓ Application completed successfully!" << std::endl;
    
    return 0;
}
