namespace Awesomium
{
	enum LogLevel {
		kLogLevel_None = 0,  ///< No log is created
		kLogLevel_Normal,    ///< Logs only errors
		kLogLevel_Verbose,   ///< Logs everything
	};

	enum Error {
		kError_None = 0,
		kError_BadParameters,
		kError_ObjectGone,
		kError_ConnectionGone,
		kError_TimedOut,
		kError_WebViewGone,
		kError_Generic,
	};

	enum JSObjectType {
		kJSObjectType_Local,
		kJSObjectType_Remote,
		kJSObjectType_RemoteGlobal,
	};

	typedef int WebViewType;
}