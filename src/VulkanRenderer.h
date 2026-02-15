#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include <windows.h>

#include <vulkan/vulkan.h>

class VulkanRenderer {
public:
    VulkanRenderer() = default;
    ~VulkanRenderer();

    VulkanRenderer(const VulkanRenderer&) = delete;
    VulkanRenderer& operator=(const VulkanRenderer&) = delete;

    void initialize(HWND hwnd, uint32_t width, uint32_t height);
    void drawFrame();
    void onResize(uint32_t width, uint32_t height);
    void shutdown();

private:
#ifdef NDEBUG
    static constexpr bool kEnableValidationLayers = false;
#else
    static constexpr bool kEnableValidationLayers = true;
#endif

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        [[nodiscard]] bool isComplete() const {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities{};
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    void createInstance();
    void setupDebugMessenger();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) const;
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createSwapChain();
    void createImageViews();
    void createRenderPass();
    void createGraphicsPipeline();
    void createFramebuffers();
    void createCommandPool();
    void createCommandBuffers();
    void createSyncObjects();
    void createPerImageSyncObjects();
    void recreateSwapChain();
    void cleanupSwapChain();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) const;

    [[nodiscard]] QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;
    [[nodiscard]] SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) const;
    [[nodiscard]] bool isDeviceSuitable(VkPhysicalDevice device) const;
    [[nodiscard]] bool checkValidationLayerSupport() const;
    [[nodiscard]] bool checkDeviceExtensionSupport(VkPhysicalDevice device) const;
    [[nodiscard]] std::vector<const char*> getRequiredExtensions() const;
    [[nodiscard]] VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
    [[nodiscard]] VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const;
    [[nodiscard]] VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;

    [[nodiscard]] static std::vector<char> readFile(const char* filename);
    [[nodiscard]] VkShaderModule createShaderModule(const std::vector<char>& code) const;

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
        void* userData
    );
    static VkResult CreateDebugUtilsMessengerEXT(
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT* createInfo,
        const VkAllocationCallbacks* allocator,
        VkDebugUtilsMessengerEXT* debugMessenger
    );
    static void DestroyDebugUtilsMessengerEXT(
        VkInstance instance,
        VkDebugUtilsMessengerEXT debugMessenger,
        const VkAllocationCallbacks* allocator
    );

    HWND hwnd_{nullptr};
    uint32_t width_{0};
    uint32_t height_{0};
    bool framebufferResized_{false};
    bool initialized_{false};
    bool validationLayersEnabled_{kEnableValidationLayers};

    VkInstance instance_{VK_NULL_HANDLE};
    VkDebugUtilsMessengerEXT debugMessenger_{VK_NULL_HANDLE};
    VkSurfaceKHR surface_{VK_NULL_HANDLE};
    VkPhysicalDevice physicalDevice_{VK_NULL_HANDLE};
    VkDevice device_{VK_NULL_HANDLE};
    VkQueue graphicsQueue_{VK_NULL_HANDLE};
    VkQueue presentQueue_{VK_NULL_HANDLE};

    VkSwapchainKHR swapChain_{VK_NULL_HANDLE};
    std::vector<VkImage> swapChainImages_;
    VkFormat swapChainImageFormat_{VK_FORMAT_UNDEFINED};
    VkExtent2D swapChainExtent_{};
    std::vector<VkImageView> swapChainImageViews_;

    VkRenderPass renderPass_{VK_NULL_HANDLE};
    VkPipelineLayout pipelineLayout_{VK_NULL_HANDLE};
    VkPipeline graphicsPipeline_{VK_NULL_HANDLE};
    std::vector<VkFramebuffer> swapChainFramebuffers_;

    VkCommandPool commandPool_{VK_NULL_HANDLE};
    std::vector<VkCommandBuffer> commandBuffers_;

    std::vector<VkSemaphore> imageAvailableSemaphores_;
    std::vector<VkSemaphore> renderFinishedSemaphores_;
    std::vector<VkFence> inFlightFences_;
    std::vector<VkFence> imagesInFlight_;
    uint32_t currentFrame_{0};

    const std::vector<const char*> validationLayers_{
        "VK_LAYER_KHRONOS_validation"
    };

    const std::vector<const char*> deviceExtensions_{
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    static constexpr uint32_t kMaxFramesInFlight = 2;
};
