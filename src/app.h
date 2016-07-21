class GarryApp : public CefApp {
public:
    void OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line) OVERRIDE {
		#ifdef TESTING
			command_line->AppendSwitch("enable-logging");
		#endif

		command_line->AppendSwitch("allow-file-access-from-files");

		command_line->AppendSwitch("enable-system-flash");

		// Fucks over everything.
		//command_line->AppendSwitch("enable-begin-frame-scheduling");

		// Doesn't seem to help much one way or the other, just leave it on so we can use webgl.
		//command_line->AppendSwitch("disable-gpu");
		//command_line->AppendSwitch("disable-gpu-compositing");
	}

	void OnRegisterCustomSchemes(CefRefPtr<CefSchemeRegistrar> registrar) OVERRIDE {
		registrar->AddCustomScheme("asset", true, true, true);
		//registrar->AddCustomScheme("call", true, false, false);
	}
private:
	IMPLEMENT_REFCOUNTING(GarryApp);
};