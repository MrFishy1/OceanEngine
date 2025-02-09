#pragma once

/**
 * @file gl_Framebuffer.hpp
 * @author Evan F.
 * @brief The implementation of an OpenGL Framebuffer.
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Types/Integers.hpp"

#include "Ocean/Primitives/Assert.hpp"
#include "Ocean/Primitives/Array.hpp"

#include "Ocean/Renderer/Framebuffer.hpp"

namespace Ocean {

    namespace Splash {

        class glFramebuffer : public Framebuffer {
        public:
            glFramebuffer(const FramebufferSpecification& spec);
            virtual ~glFramebuffer();

            virtual void Bind() override final;
            virtual void Unbind() override final;

            virtual void Invalidate() override final;

            virtual void Resize(u32 width, u32 height) override final;
            
            virtual u32 ReadPixel(u32 attachmentIndex, i32 x, i32 y) override final;

            inline virtual u32 GetColorAttachmentID(u32 index = 0) const override final { OASSERT_LENGTH(index, this->m_ColorAttachments.size()); return this->m_ColorAttachments[index]; }
            virtual void ClearAttachment(u32 attachmentIndex, i32 value) override final;

        private:
            u32 m_RendererID;

            DynamicArray<u32> m_ColorAttachments;
            u32 m_DepthAttachment;

        };  // Framebuffer

    }   // Splash

}   // Ocean
