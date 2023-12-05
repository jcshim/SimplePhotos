#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.
#include "ImageFileInfo.h"
namespace winrt
{
    using namespace Windows::ApplicationModel;
    using namespace Windows::Foundation;
    using namespace Windows::Foundation::Collections;
    using namespace Windows::Storage;
    using namespace Windows::Storage::Search;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
}
namespace winrt::SimplePhotos::implementation
{
    MainWindow::MainWindow() :
        m_images(winrt::single_threaded_observable_vector<IInspectable>())
    {
        InitializeComponent();
        GetItemsAsync();
    }
    void MainWindow::ImageGridView_ContainerContentChanging(ListViewBase const& sender, ContainerContentChangingEventArgs const& args)
    {
        if (args.InRecycleQueue())
        {
            auto templateRoot = args.ItemContainer().ContentTemplateRoot().try_as<Grid>();
            auto image = templateRoot.FindName(L"ItemImage").try_as<Image>();
            image.Source(nullptr);
        }
        if (args.Phase() == 0)
        {
            args.RegisterUpdateCallback({ this, &MainWindow::ShowImage });
            args.Handled(true);
        }
    }

    fire_and_forget MainWindow::ShowImage(ListViewBase const& sender, ContainerContentChangingEventArgs const& args)
    {
        if (args.Phase() == 1)
        {
            // It's phase 1, so show this item's image.
            auto templateRoot = args.ItemContainer().ContentTemplateRoot().try_as<Grid>();
            auto image = templateRoot.FindName(L"ItemImage").try_as<Image>();
            auto item = args.Item().try_as<SimplePhotos::ImageFileInfo>();
            image.Source(co_await get_self<SimplePhotos::implementation::ImageFileInfo>(item)->GetImageThumbnailAsync());
        }
    }
    IAsyncAction MainWindow::GetItemsAsync()
    {
        StorageFolder appInstalledFolder = Package::Current().InstalledLocation();
        StorageFolder picturesFolder = co_await appInstalledFolder.GetFolderAsync(L"Assets\\Samples");

        auto result = picturesFolder.CreateFileQueryWithOptions(QueryOptions());

        IVectorView<StorageFile> imageFiles = co_await result.GetFilesAsync();
        for (auto&& file : imageFiles)
        {
            Images().Append(co_await LoadImageInfoAsync(file));
        }

        ImageGridView().ItemsSource(Images());
    }

    IAsyncOperation<SimplePhotos::ImageFileInfo> MainWindow::LoadImageInfoAsync(StorageFile file)
    {
        auto properties = co_await file.Properties().GetImagePropertiesAsync();
        SimplePhotos::ImageFileInfo info(properties,
            file, file.DisplayName(), file.DisplayType());
        co_return info;
    }

}
