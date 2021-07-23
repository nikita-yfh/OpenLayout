#ifndef LAYERINFO_H
#define LAYERINFO_H

//(*Headers(LayerInfo)
#include <wx/dialog.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
//*)

class LayerInfo: public wxDialog
{
	public:

		LayerInfo(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~LayerInfo();

		//(*Declarations(LayerInfo)
		wxPanel* Panel1;
		wxPanel* Panel2;
		wxPanel* Panel3;
		wxPanel* Panel4;
		wxPanel* Panel5;
		wxStaticBitmap* StaticBitmap1;
		wxStaticBitmap* StaticBitmap2;
		wxStaticBitmap* StaticBitmap3;
		wxStaticBitmap* StaticBitmap4;
		wxStaticBitmap* StaticBitmap5;
		//*)

	protected:

		//(*Identifiers(LayerInfo)
		static const long ID_STATICBITMAP1;
		static const long ID_PANEL1;
		static const long ID_STATICBITMAP2;
		static const long ID_PANEL2;
		static const long ID_STATICBITMAP3;
		static const long ID_PANEL3;
		static const long ID_STATICBITMAP4;
		static const long ID_PANEL4;
		static const long ID_STATICBITMAP5;
		static const long ID_PANEL5;
		//*)

	private:

		//(*Handlers(LayerInfo)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
