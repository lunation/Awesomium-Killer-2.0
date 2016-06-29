class GarryApp : public CefApp {
public:
    void OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line) OVERRIDE {
		command_line->AppendSwitch("enable-logging");
		command_line->AppendSwitchWithValue("v", "1");
	}
private:
	IMPLEMENT_REFCOUNTING(GarryApp);
};