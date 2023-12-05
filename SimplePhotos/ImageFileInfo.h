// ImageFileInfo.h
#pragma once
#include "ImageFileInfo.g.h"

namespace winrt::SimplePhotos::implementation
{
    struct ImageFileInfo : ImageFileInfoT<ImageFileInfo>
    {
        ImageFileInfo() = default;

        ImageFileInfo(
            winrt::Windows::Storage::FileProperties::ImageProperties const& properties,
            winrt::Windows::Storage::StorageFile const& imageFile,
            hstring const& name,
            hstring const& type);

        winrt::Windows::Storage::FileProperties::ImageProperties ImageProperties()
        {
            return m_imageProperties;
        }

        winrt::Windows::Storage::StorageFile ImageFile()
        {
            return m_imageFile;
        }

        hstring ImageName()
        {
            return m_imageName;
        }

        hstring ImageFileType()
        {
            return m_imageFileType;
        }

        hstring ImageDimensions()
        {
            return to_hstring(ImageProperties().Width()) + L" x " + to_hstring(ImageProperties().Height());
        }

        hstring ImageTitle()
        {
            return ImageProperties().Title() == L"" ? ImageName() : ImageProperties().Title();
        }

        void ImageTitle(hstring const& value);

        uint32_t ImageRating()
        {
            return ImageProperties().Rating();
        }

        void ImageRating(uint32_t value);

        winrt::event_token PropertyChanged(winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
        {
            return m_propertyChanged.add(handler);
        }

        void PropertyChanged(winrt::event_token const& token) noexcept
        {
            m_propertyChanged.remove(token);
        }

        Windows::Foundation::IAsyncOperation<Microsoft::UI::Xaml::Media::Imaging::BitmapImage> GetImageSourceAsync();

        Windows::Foundation::IAsyncOperation<Microsoft::UI::Xaml::Media::Imaging::BitmapImage> GetImageThumbnailAsync();

    private:
        // File and information fields.
        Windows::Storage::FileProperties::ImageProperties m_imageProperties{ nullptr };
        Windows::Storage::StorageFile m_imageFile{ nullptr };
        hstring m_imageName;
        hstring m_imageFileType;

        event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;
        void OnPropertyChanged(hstring propertyName)
        {
            m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(propertyName));
        }
    };
}
namespace winrt::SimplePhotos::factory_implementation
{
    struct ImageFileInfo : ImageFileInfoT<ImageFileInfo, implementation::ImageFileInfo>
    {
    };
}