#ifndef _CLIENT_WINDOW_H_
#define _CLIENT_WINDOW_H_

#include "../GuiLibrary/GuiWindow.h"
#include "Sockets.h"


class ConnectionWindow;
class GuiPanel;
class GuiLabel;
class GuiButton;
class GuiStatusBar;
class PlaybackTrackerPanel;
class ButtonPanel;
class GuiScrollList;
class MessageQueue;
class MicReader;

class ClientWindow : public GuiWindow
{
public:
	ClientWindow(HINSTANCE hInst);
	virtual ~ClientWindow();
	virtual void onCreate();

	void addRemoteFile(LPWSTR filename);

private:
	static void onClickPlay(void*);
	static bool onClickMic(GuiComponent *_pThis, UINT command, UINT id, WPARAM wParam, LPARAM lParam, INT_PTR *retval);
	static bool onMicStop(GuiComponent *_pThis, UINT command, UINT id, WPARAM wParam, LPARAM lParam, INT_PTR *retval);

	bool recording;
	bool requestingRecorderStop;
	MessageQueue *micMQueue;
	MicReader *micReader;

	HBITMAP playButtonUp;
	HBITMAP playButtonDown;
	HBRUSH darkBackground;
	HBRUSH lightBackground;
	HBRUSH accentBrush;
	HPEN borderPen;
	HPEN nullPen;
	COLORREF textColor;

	GuiPanel *topPanel;
	GuiPanel *topPanelStretch;
	GuiScrollList *fileContainerPanel;
	GuiPanel *seekPanel;
	GuiLabel *micTargetLabel;
	GuiButton *micTargetButton;
	GuiStatusBar *statusBar;
	PlaybackTrackerPanel *trackerPanel;
	GuiPanel *buttonSpacer1;
	GuiPanel *buttonSpacer2;
	GuiPanel *bottomSpacer;
	ButtonPanel *playButton;

	UDPSocket* udpSock;
};

#endif
