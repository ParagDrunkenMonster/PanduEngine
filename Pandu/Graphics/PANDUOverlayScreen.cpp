#include "PANDUOverlayScreen.h"

namespace Pandu
{
	OverlayScreen::OverlayScreen(const String& _name,const Rect& _rect, const Rect& _imageRectTexCoord)
		: Overlay(_name)
		, m_OverlayRect(_rect)
		, m_ImageRect(_imageRectTexCoord)
		, m_DirtyImageRect(true)
	{
		Construct();
	}

	OverlayScreen::~OverlayScreen()
	{
	}

	void OverlayScreen::PreRenderUpdate()
	{
	}

	void OverlayScreen::Construct()
	{
		float vertices[] = {
								m_OverlayRect.m_Left							, m_OverlayRect.m_Bottom + m_OverlayRect.m_Height		, 0.0f,
								m_OverlayRect.m_Left + m_OverlayRect.m_Width	, m_OverlayRect.m_Bottom + m_OverlayRect.m_Height		, 0.0f,
								m_OverlayRect.m_Left + m_OverlayRect.m_Width	, m_OverlayRect.m_Bottom								, 0.0f,
								m_OverlayRect.m_Left							, m_OverlayRect.m_Bottom								, 0.0f,
							};

		unsigned int indices[] = {
					2,1,0,
					3,2,0,
					};
		
		m_OverlayMesh->Lock();
		
		float texCoords[] = {
								m_ImageRect.m_Left							, m_ImageRect.m_Bottom + m_ImageRect.m_Height		,
								m_ImageRect.m_Left + m_ImageRect.m_Width	, m_ImageRect.m_Bottom + m_ImageRect.m_Height		,
								m_ImageRect.m_Left + m_ImageRect.m_Width	, m_ImageRect.m_Bottom								,
								m_ImageRect.m_Left							, m_ImageRect.m_Bottom								,
							};
		
		m_OverlayMesh->SetVertexBuffer(vertices, 12);
		m_OverlayMesh->SetIndexBuffer(indices, 6);
		m_OverlayMesh->SetTexCoordBuffer(texCoords,4 * 2);

		m_OverlayMesh->Unlock();
	}
}