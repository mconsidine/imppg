/*
ImPPG (Image Post-Processor) - common operations for astronomical stacks and other images
Copyright (C) 2016-2019 Filip Szczerek <ga.software@yahoo.com>

This file is part of ImPPG.

ImPPG is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ImPPG is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ImPPG.  If not, see <http://www.gnu.org/licenses/>.

File description:
    CPU & bitmaps back end declaration.
*/

#include <functional>
#include <optional>
#include <wx/bitmap.h>
#include <wx/timer.h>

#include "../backend.h"
#include "../../common.h"

namespace imppg::backend {

class c_CpuAndBitmaps: public IBackEnd
{
public:
    c_CpuAndBitmaps(wxScrolledCanvas& imgView);

    // Events -------------------------------------------------
    void ImageViewScrolledOrResized(float zoomFactor) override;
    void ImageViewZoomChanged(float zoomFactor) override;
    void FileOpened(c_Image&& img) override;

private:
    wxScrolledCanvas& m_ImgView;
    std::optional<c_Image> m_Img;
    std::optional<wxBitmap> m_ImgBmp; ///< Bitmap which wraps `m_Img` for displaying on `m_ImgView`.
    float m_ZoomFactor{ZOOM_NONE};
    float m_NewZoomFactor{ZOOM_NONE};
    std::optional<wxBitmap> m_BmpScaled; ///< Currently visible scaled fragment (or whole) of `m_ImgBmp`.
    wxRect m_ScaledArea; ///< Area within `m_ImgBmp` represented by `m_BmpScaled`.

    void OnPaint(wxPaintEvent& event);
    void CreateScaledPreview(float zoomFactor);

    class c_ScalingTimer: public wxTimer
    {
        std::function<void()> m_Handler;
    public:
        void SetHandler(std::function<void()> handler) { m_Handler = handler; }
        void Notify() override { m_Handler(); }
    } m_ScalingTimer;
};

} // namespace imppg::backend