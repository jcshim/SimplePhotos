// ImageFileInfo.cpp
#include "pch.h"
#include "ImageFileInfo.h"
#include "ImageFileInfo.g.cpp"
#include <random>

using namespace std;
namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Media::Imaging;
    using namespace Windows::Foundation;
    using namespace Windows::Storage;
    using namespace Windows::Storage::FileProperties;
    using namespace Windows::Storage::Streams;
}

namespace winrt::SimplePhotos::implementation
{
    ImageFileInfo::ImageFileInfo(
        winrt::Windows::Storage::FileProperties::ImageProperties const& properties,
        winrt::Windows::Storage::StorageFile const& imageFile,
        hstring const& name,
        hstring const& type) :
        m_imageProperties{ properties },
        m_imageName{ name },
        m_imageFileType{ type },
        m_imageFile{ imageFile }
    {
        auto rating = properties.Rating();
        auto random = std::random_device();
        std::uniform_int_distribution<> dist(1, 5);
        ImageRating(rating == 0 ? dist(random) : rating);
    }

    void ImageFileInfo::ImageTitle(hstring const& value)
    {
        if (ImageProperties().Title() != value)
        {
            ImageProperties().Title(value);
            ImageProperties().SavePropertiesAsync();
            OnPropertyChanged(L"ImageTitle");
        }
    }

    void ImageFileInfo::ImageRating(uint32_t value)
    {
        if (ImageProperties().Rating() != value)
        {
            ImageProperties().Rating(value);
            ImageProperties().SavePropertiesAsync();
            OnPropertyChanged(L"ImageRating");
        }
    }

    IAsyncOperation<BitmapImage> ImageFileInfo::GetImageSourceAsync()
    {
        IRandomAccessStream stream{ co_await ImageFile().OpenAsync(FileAccessMode::Read) };
        BitmapImage bitmap{};
        bitmap.SetSource(stream);
        co_return bitmap;
    }

    IAsyncOperation<BitmapImage> ImageFileInfo::GetImageThumbnailAsync()
    {
        auto thumbnail = co_await m_imageFile.GetThumbnailAsync(FileProperties::ThumbnailMode::PicturesView);
        BitmapImage bitmapImage{};
        bitmapImage.SetSource(thumbnail);
        thumbnail.Close();
        co_return bitmapImage;
    }
}