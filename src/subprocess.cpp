#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define OS_WIN
#include "include/cef_app.h"

#include "src/app.h"
#include "src/js_interface.h"

#include <unordered_map>
#include <vector>

class JsObjId : public CefBase {
public:
	JsObjId(int i) { id = i; }
	int id;
private:
	IMPLEMENT_REFCOUNTING(JsObjId);
};

class GlobalInitRecord {
public:
	int parent_id;
	CefString key;
	JsValueType type;
};

struct BrowserData {
	std::vector<GlobalInitRecord*> global_data;
};

std::unordered_map<int, BrowserData> browser_table;

class SubApp : public GarryApp, public CefRenderProcessHandler {
public:
	virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE {
		return this;
	}

	virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) OVERRIDE {
		auto args = message->GetArgumentList();

		if (message->GetName() == "garry_js_set_global") {

			int parent_id = args->GetInt(0);
			CefString key = args->GetString(1);

			GlobalInitRecord* gir;

			switch (args->GetInt(2)) {

			case JsValueType::Object:
			case JsValueType::CustomMethod:
				gir = new GlobalInitRecord();
				gir->parent_id = parent_id;
				gir->key = key;
				gir->type =	static_cast<JsValueType>(args->GetInt(2));
				
				browser_table[browser->GetIdentifier()].global_data.push_back( gir );
				break;
			default:
				cef_log(0, 0, 0, "UNKNOWN GLOBAL TYPE");
			}

			return true;
		}

		/*if (message->GetName() == "garry_js_index_obj") {
			auto args = message->GetArgumentList();
			int id = args->GetInt(0);
			CefString key = args->GetString(1);

			if (id == 1) {
				auto context = browser->GetMainFrame()->GetV8Context();

				context->Enter();

				auto glob = context->GetGlobal();

				auto val = glob->GetValue(key);

				val->IsValid();

				auto msg = CefProcessMessage::Create("garry_js_result_value");
				auto args = msg->GetArgumentList();
				
				if (val->IsObject()) {
					args->SetInt(0, JsValueType::Object );
					int id = getObjectId(val);
					args->SetInt(1, id);
				}
				else if (val->IsUndefined()) { 
					args->SetInt(0, JsValueType::Undefined );
				}
				else {
					cef_log(0, 0, 0, ("Unknown type: " + val->GetStringValue().ToString()).c_str() );
					args->SetInt(0, JsValueType::Unknown );
				}

				context->Exit();

				browser->SendProcessMessage(PID_BROWSER, msg);
			}
			else {
				cef_log(0, 0, 0, ( "index " + std::to_string(id) +  " " + key.ToString()).c_str() );
			}

			return true;
		}
		/*auto x = message->GetName();
		cef_log(0, 0, 0, x.ToString().c_str());*/
		return false;
	}

	void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) OVERRIDE {
		
		std::vector<GlobalInitRecord*>& data = browser_table[browser->GetIdentifier()].global_data;
		std::vector<CefRefPtr<CefV8Value>> global_build_array;

		CefRefPtr<CefV8Value> makeFunc;
		CefRefPtr<CefV8Exception> err;

		// Function MUST be wrapped in parens or it will fail to compile (wtf?)
		bool success = context->Eval(R"(
			(function(id,name) {
				return function() {
					if (window.location.protocol != "asset:")
						return;
					var request = new XMLHttpRequest();
					request.open("POST", "asset://call/"+id+"/"+name+"/"+JSON.stringify(Array.prototype.slice.call(arguments)), false);
					request.send();
					if (request.status==200) {
						return JSON.parse(request.responseText);
					} else {
						throw new Error("REMOTE CALL FAILED");
					}
				}
			})
		)", makeFunc, err);

		auto global = context->GetGlobal();

		auto makeFuncArgs = CefV8ValueList(2);

		for (int i = 0; i < data.size(); i++) {

			CefRefPtr<CefV8Value> new_value;

			switch (data[i]->type) {
			case Object:
				new_value = CefV8Value::CreateObject(nullptr);
				break;
			case CustomMethod:
				makeFuncArgs[0] = CefV8Value::CreateInt(data[i]->parent_id);

				makeFuncArgs[1] = CefV8Value::CreateString(data[i]->key);

				new_value = makeFunc->ExecuteFunction(nullptr, makeFuncArgs);
				break;
			default:
				cef_log(0, 0, 0, "???");
				break;
			}
			
			if (data[i]->parent_id == -1) {
				auto current = global->GetValue(data[i]->key);
				
				if (new_value->IsObject() && current->IsObject()) {
					new_value = current;
				}
				else
					global->SetValue(data[i]->key,  new_value, cef_v8_propertyattribute_t::V8_PROPERTY_ATTRIBUTE_NONE ); // not sure about this property thing, check back later!
			}
			else {
				auto current = global_build_array[data[i]->parent_id]->GetValue(data[i]->key);

				global_build_array[data[i]->parent_id]->SetValue(data[i]->key, new_value, cef_v8_propertyattribute_t::V8_PROPERTY_ATTRIBUTE_NONE); // not sure about this property thing, check back later!
			}

			global_build_array.push_back(new_value);
		}
	}

	/*bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) OVERRIDE {
		cef_log(0, 0, 0, ("ran func " + name.ToString()).c_str() );

		return true;
	}*/
private:
	IMPLEMENT_REFCOUNTING(SubApp);
};

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
) {
	//cef_log(0, 0, 0, "proc start");
	CefMainArgs args(hInstance);
	return CefExecuteProcess(args, new SubApp(), nullptr);
}