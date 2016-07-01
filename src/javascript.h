namespace Awesomium {

	class JSObject;
	class JSArray;

	// Inheritance is for poons.
	class JSValue_Internal {
	public:
		JSValue_Internal() { }
		JSValue_Internal(const JSValue_Internal& other) {
			if (other.type == Object) {
				setObject(*other.data.object);
			}
			else if (other.type == Array) {
				setArray(*other.data._array);
			}
			else if (other.type == String) {
				setString(*other.data.string);
			}
			else {
				data = other.data;
				type = other.type;
			}

		}

		~JSValue_Internal() {
			reset();
		}

		void reset() {

			if (type == Object || type == String || type == Array) {
				delete data.ptr;
			}
		}

		JSObject* getObject() {
			if (type != Object)
				panic("tried to get object from nonobject");
			return data.object;
		}
		void setObject(const JSObject& val);

		JSArray* getArray() {
			if (type != Array)
				panic("tried to get array from nonarray");
			return data._array;
		}
		void setArray(const JSArray& val);

		WebString getString() {
			//debug_log("a");
			if (type != String)
				panic("tried to get string from nonstring");
			/*debug_log("b");
			if (data_string != nullptr)
				debug_log("c");
			if (data_string->data() != nullptr)
				debug_log("d");
*/
			//debug_log(  (CefString( data_string->data() ).ToString() + " " + std::to_string(data_string->length())) .c_str() );
			return *data.string;
		}
		void setString(const WebString& val) {
			reset();
			type = String;
			data.string = new WebString(val);
		}
		

		void setNumber(double n) {
			reset();
			type = Number;
			data.number = n;
		}
		double getNumber() {
			if (type != Number)
				panic("tried to get number from nonnumber");
			return data.number;
		}

		void setBool(bool val) {
			reset();
			type = Boolean;
			data._bool = val;
		}
		bool getBool() {
			if (type != Boolean)
				panic("tried to get bool from nonbool");
			return data._bool;
		}


		bool isType(JsValueType t) {
			return type == t;
		}

		std::string toJSON() {
			switch (type) {
			case Undefined:
				return "null";
			case Boolean:
				return data._bool ? "true" : "false";
			case String:
				return data.string->quote(); // QUOTE THIS
			default:
				//debug_stream << type << std::endl;
				panic("cant take ? to json");
			}
			return "";
		}
		
	private:
		union {
			void* ptr;
			JSObject* object;
			JSArray* _array;
			WebString* string;
			double number;
			bool _bool;
		} data;

		JsValueType type = Undefined;
	};

	class DllExport JSValue
	{
	public:

		JSValue() {
			value = new JSValue_Internal();
		};
		explicit JSValue(bool val) {
			value = new JSValue_Internal();
			value->setBool(val);
		};
		explicit JSValue(int val) {
			value = new JSValue_Internal();
			value->setNumber(val);
		};
		explicit JSValue(double val) {
			value = new JSValue_Internal();
			value->setNumber(val);
		};
		JSValue(const WebString& val) {
			value = new JSValue_Internal();
			value->setString(val);
		};
		JSValue(const JSObject& val) {
			value = new JSValue_Internal();
			value->setObject(val);
		};
		JSValue(const JSArray &val) {
			value = new JSValue_Internal();
			value->setArray(val);
		};
		
		JSValue(const JSValue& orig) {
			value = new JSValue_Internal(*orig.value);
		};

		JSValue& operator=(const JSValue&rhs) {
			delete value;
			value = new JSValue_Internal(*rhs.value);
			return *this;
		}; // we PROBABLY just want shallow copies (copy the handle only)

		~JSValue() {
			delete value;
		};


		// Not sure wtf I implemented these, going to tunnel through CEF anyway...
		static const JSValue& Undefined() {
			debug_log(__FUNCTION__);
			static JSValue* undef = nullptr;
			if (undef == nullptr) {
				undef = new JSValue();
			}
			return *undef;
		};
		static const JSValue& Null() {
			debug_log(__FUNCTION__);
			static JSValue* _null = nullptr;
			if (_null == nullptr) {
				_null = new JSValue();
			}
			return *_null;
		};

		bool IsBoolean() const { debug_log(__FUNCTION__); return value->isType(Boolean); };
		bool IsInteger() const {
			debug_log(__FUNCTION__);
			double y;
			return	value->isType(Number) && modf( value->getNumber(), &y ) == 0;
		};
		bool IsDouble() const { return value->isType(Number); };
		bool IsNumber() const { return value->isType(Number); };
		bool IsString() const { return value->isType(String); };
		bool IsArray() const { value->isType(Array); return value->isType(Array); };
		bool IsObject() const { return value->isType(Object); };
		bool IsNull() const { debug_log(__FUNCTION__); return false; };
		bool IsUndefined() const { return value->isType(JsValueType::Undefined); };

		WebString ToString() const {
			auto str = value->getString();
			//debug_log(",,");
			//debug_log( CefString(str.data()).ToString().c_str() );
			return str;
		};
		int ToInteger() const { debug_log(__FUNCTION__); return int(value->getNumber()); };
		double ToDouble() const { return value->getNumber(); };
		bool ToBoolean() const { return value->getBool(); };

		JSArray& ToArray() {
			return *(value->getArray());
		}
		const JSArray& ToArray() const {
			return *(value->getArray());
		}

		JSObject& ToObject() {
			return *(value->getObject());
		}
		const JSObject& ToObject() const {
			return *(value->getObject());
		}

		static bool ParseJSON(const wchar_t*& str, JSValue& x);
		
		std::string ToJSON() {
			return value->toJSON();
		}
	private:
		JSValue_Internal* value;
	};

	/*bool js_ipc_complete = false;
	JSValue js_ipc_result;

	JSValue fetch_me_some_js(CefRefPtr<CefBrowser> browser, int id, const CefString& key) {
		auto msg = CefProcessMessage::Create("garry_js_index_obj");
		auto args = msg->GetArgumentList();

		args->SetInt(0, id);
		args->SetString(1, key);

		js_ipc_complete = false;

		browser->SendProcessMessage(PID_RENDERER, msg);

		while (!js_ipc_complete)
			CefDoMessageLoopWork();

		return js_ipc_result;
	}*/

	template<class T>
	class WebVector;

	class DllExport JSArray
	{
	public:
		explicit JSArray() {
			vector = new std::vector<JSValue>();
		};
		JSArray(unsigned int n) { debug_log("NEW JSARRAY2"); };
		JSArray(const JSArray &other) {
			vector = new std::vector<JSValue>(*other.vector);
		};
		JSArray & operator=(const JSArray&other) {
			delete vector;
			vector = new std::vector<JSValue>(*other.vector);
			return *this;
		};

		~JSArray() {
			delete vector;
		}

		unsigned int size() const {
			return vector->size();
		};

		unsigned int capacity() const {
			debug_log(__FUNCTION__);
			return 1;
		};
		
		JSValue & At(unsigned int idx) {
			return vector->at(idx);
		};
		
		const JSValue & At(unsigned int idx) const {
			return vector->at(idx);
		};

		JSValue & operator[](unsigned int idx) {
			return (*vector)[idx];
		};

		const JSValue & operator[](unsigned int idx) const {
			return (*vector)[idx];
		}

		void Push(const JSValue&item) {
			vector->push_back(item);
		};
		void Pop() { debug_log(__FUNCTION__); };
		void Insert(const JSValue&item, unsigned int idx) { debug_log(__FUNCTION__); };
		void Erase(unsigned int idx) { debug_log(__FUNCTION__); };
		void Clear() { debug_log(__FUNCTION__); };

		static bool ParseJSON(const wchar_t*& str, JSArray& x) {

			if (*str++ != '[')
				panic("tried to parse an array. IT AINT A FUCKIN ARRAY.");

			x = JSArray();

			for (;;) {
				if (*str == ']')
					break;

				JSValue y;
				if (!JSValue::ParseJSON(str, y)) {
					debug_log("FAILED TO PARSE ARRAY VALUE!");
					return false;
				}

				if (*str == ',') {
					debug_log("skip comma");
					str++;
				}

				x.Push(y);
			}

			//debug_stream << (char)*str << std::endl;
			return true;
		}

	protected:
		//WebVector<JSValue>* vector_;
		std::vector<JSValue>* vector;
		//JSValue value;
	};

	class WebView;

	class JSObject_Internal {
	
	public:
		virtual JSObject_Internal* clone() = 0;
		virtual void copyTo(JSObject& target) = 0;

		virtual int getRemoteId() = 0;

		virtual JSValue get(const WebString& name) const = 0;
		virtual void set(const WebString& name, const JSValue& value, bool wait) = 0;
		virtual bool has(const WebString& name) = 0;

		virtual void setCustomMethod(const WebString& name, bool has_return_value) {
			panic("ONLY GLOBALS SUPPORT CUSTOM METHODS");
		}
	};

	class JSObject_Local : public JSObject_Internal {
	
	public:
		JSObject_Internal* clone() OVERRIDE {
			auto copy = new JSObject_Local();
			return copy;
		}

		void copyTo(JSObject& target) OVERRIDE {
			for (auto pair : table) {
				debug_log("COPY ELEMENT");
			}
		}
		
		int getRemoteId() OVERRIDE {
			return 0;
		}

		JSValue get(const WebString& name) const OVERRIDE {
			auto iter = table.find(name.data());

			if (iter != table.end()) {
				return iter->second;
			}

			return JSValue(); // return undefined
		}

		void set(const WebString& name, const JSValue& value, bool wait) OVERRIDE {
			table[name.data()] = value;
		}

		bool has(const WebString& name) OVERRIDE {
			return table.find(name.data()) != table.end();
		}
	private:
		std::unordered_map<std::wstring, JSValue> table;
	};

	class JSObject_Global : public JSObject_Local {
	
	public:
		JSObject_Global(WebView* browser, int n) {
			this->browser = browser;
			index = n;
		}

		JSObject_Internal* clone() OVERRIDE {
			auto copy = new JSObject_Global(browser, index);
			return copy;
		}
		
		int getRemoteId() OVERRIDE {
			return index;
		}

		JSValue get(const WebString& name) const OVERRIDE {
			return JSObject_Local::get(name);
		}

		void set(const WebString& name, const JSValue& value, bool wait) OVERRIDE;

		void setCustomMethod(const WebString& name, bool has_return_value) OVERRIDE;
	private:
		WebView* browser;
		int index;
	};

	class DllExport JSObject
	{
	public:
		JSObject() {
			object = new JSObject_Local();
		};
		JSObject(const JSObject &other) {
			object = other.object->clone();
		};
		JSObject &operator=(const JSObject& other) {
			delete object;
			object = other.object->clone();
			return *this;
		};

		unsigned int remote_id() const {
			return object->getRemoteId();
		};

		int ref_count() const { debug_log("ref count"); return 0; };

		JSObjectType type() const { debug_log(__FUNCTION__); return JSObjectType::kJSObjectType_Local; };
		WebView* owner() const { debug_log(__FUNCTION__); return nullptr; };

		JSArray GetPropertyName() const { debug_log(__FUNCTION__); return *(new JSArray()); }; // DUMB!!!
		bool HasProperty(const WebString& name) const {
			return object->has(name);
		}
		JSValue GetProperty(const WebString& name) const {
			return object->get(name);
		 };
		void SetProperty(const WebString& name, const JSValue& value) {
			object->set(name, value, true);
		};
		void SetPropertyAsync(const WebString& name, const JSValue& value) {
			object->set(name, value, false);
		};
		void RemoveProperty(const WebString& name) { debug_log(__FUNCTION__); };
		JSArray GetMethodNames() const { debug_log(__FUNCTION__); return *(new JSArray()); }; // DUMB
		bool HasMethod(const WebString& name) const { debug_log(__FUNCTION__); return false; };
		JSValue Invoke(const WebString& name, const JSArray& args) { debug_log(__FUNCTION__); return *(new JSValue()); }; // DUMB
		void InvokeAsync(const WebString& name, const JSArray& args) { debug_log(__FUNCTION__); };
		WebString ToString() const { debug_log(__FUNCTION__); return WebString(); };

		void SetCustomMethod(const WebString& name, bool has_return_value) {
			object->setCustomMethod(name, has_return_value);
		};

		Error last_error() const { debug_log(__FUNCTION__); return Error::kError_None; };

		void copyTo(JSObject& target) const {
			object->copyTo(target);
		}

		JSObject(WebView* browser, int n) {
			object = new JSObject_Global(browser, n);
		};
	private:
		JSObject_Internal* object;
	};

	// IMPL -- SUPER DUMB!

	void JSValue_Internal::setObject(const JSObject& val) {
		reset();
		type = Object;
		data.object = new JSObject(val);
	}

	void JSValue_Internal::setArray(const JSArray& val) {
		reset();
		type = Array;
		data._array = new JSArray(val);
	}

	////


	bool JSValue::ParseJSON(const wchar_t*& str, JSValue& x) {

		if (*str == '-' || isdigit(*str)) {
			// THIS IS INCOMPLETE
			std::vector<char> stringbuilder;
			stringbuilder.push_back(*str);
			str++;

			while (isdigit(*str)) {
				stringbuilder.push_back(*str);
				str++;
			}

			stringbuilder.push_back(0);

			x = JSValue(std::stod(stringbuilder.data()));
		}
		else {
			switch (*str) {
			case '[': {
				JSArray y;
				if (!JSArray::ParseJSON(str, y))
					return false;
				
				x = JSValue(y);
				
				break;
			}
			case '"': {
				WebString y;
				if (!WebString::ParseJSON(str, y))
					return false;

				x = JSValue(y);

				break;
			}
			default:

				debug_log("JSON: INVALID CHAR");
				//debug_stream << *str << std::endl;
				return false;
			}
		}

		return true;
	}

}