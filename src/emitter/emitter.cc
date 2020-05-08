#ifndef SRC_EVENTEMITTER
#define SRC_EVENTEMITTER

#include <nan.h>
#include <unordered_map>

/**
 * 
 * Native EventEmitter
 * warning: user-defined events won't catched by interceptor!
 * 
 */

class EventEmitter : Nan::AsyncResource
{
private:
  //typedefs
  typedef std::vector<Nan::Callback *> EventHandlers;
  typedef std::unordered_map<std::string, EventHandlers> EventsCollection;
  //structs
  struct Wrap
  {
    Nan::Persistent<v8::Object> emitter;
    EventsCollection Events;
    const char *name;
  };
  struct Arguments
  {
    size_t Length;
    v8::Local<v8::Value> *Data;
  };
  //variables
  inline static Nan::Persistent<v8::Object> interceptor = Nan::New<v8::Object>();
  Wrap *internal = new Wrap;

  //retrieve internal values inside functions
  static Wrap *unwrap(Nan::NAN_METHOD_ARGS_TYPE info)
  {
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    return static_cast<Wrap *>(wrap->Value());
  }

  //Shift passed arguments
  //Needed for remove event name from arguments
  static Arguments shift(Nan::NAN_METHOD_ARGS_TYPE info)
  {
    size_t argc = info.Length() - 1;
    v8::Local<v8::Value> *argv{new v8::Local<v8::Value>[argc]};
    for (size_t i = 0; i < argc; i++)
      argv[i] = info[i + 1];
    return Arguments{argc, argv};
  }

  //-----------------------------------------------------------------------------
  // $on
  //-----------------------------------------------------------------------------
  static NAN_METHOD($on)
  {
    //arguments
    std::string eventName = *(Nan::Utf8String(info[0]));
    v8::Local<v8::Function> cb = info[1].As<v8::Function>();
    //persistent function callback
    Nan::Callback *callback = new Nan::Callback(cb);
    //check if have internal wrapper
    if (info.This()->InternalFieldCount() > 0)
    {
      Wrap *internal = unwrap(info);
      if (internal->Events.find(eventName) == internal->Events.end())
      {
        EventHandlers handlers;
        handlers.push_back(callback);
        internal->Events.insert({eventName, handlers});
      }
      else
      {
        internal->Events.at(eventName).push_back(callback);
      }
    }
    else
    {
      //function called without wrapper
      printf("[EventEmitter - %s] trying to register event without internal wrapper!\n", eventName.c_str());
    }
  }

  //-----------------------------------------------------------------------------
  // $emit
  //-----------------------------------------------------------------------------
  static NAN_METHOD($emit)
  {
    //arguments
    std::string eventName = *(Nan::Utf8String(info[0]));
    //check if have internal wrapper
    if (info.This()->InternalFieldCount() > 0)
    {
      Wrap *internal = unwrap(info);
      //fill this!!!
      //printf("[EventEmitter - %s] trying to call event without internal wrapper!\n", eventName.c_str());

      auto handlers = internal->Events.find(eventName);
      if (handlers != internal->Events.end())
      {
        Arguments args = shift(info);
        for (auto handler : handlers->second)
        {
          Nan::Call(*handler, args.Length, args.Data);
        }
      }
      else
        printf("[%s] handler NOT found!\n", eventName.c_str());
    }
    else
    {
      //function called without wrapper
      printf("[EventEmitter - %s] trying to call event without internal wrapper!\n", eventName.c_str());
    }
  }

public:
  //Constructor
  EventEmitter(const char *name = "EventEmitter") : Nan::AsyncResource(name)
  {
    v8::Isolate *isolate = v8::Isolate::GetCurrent();
    v8::Local<v8::Context> context = Nan::GetCurrentContext();
    //define tpl
    v8::Local<v8::ObjectTemplate> tpl = Nan::New<v8::ObjectTemplate>();
    //set internal fields count
    tpl->SetInternalFieldCount(1);
    //set methods
    tpl->Set(Nan::New("$on").ToLocalChecked(), Nan::New<v8::FunctionTemplate>($on));
    tpl->Set(Nan::New("$emit").ToLocalChecked(), Nan::New<v8::FunctionTemplate>($emit));
    //create instance
    v8::Local<v8::Object> obj = tpl->NewInstance(context).ToLocalChecked();
    //wrap EventsCollection
    internal->emitter.Reset(obj);
    internal->name = name;
    obj->SetInternalField(0, v8::External::New(isolate, internal));
  }
  ~EventEmitter() {}

  //catch all events
  static void SetInterceptor(Nan::ADDON_REGISTER_FUNCTION_ARGS_TYPE target) { interceptor.Reset(target); }

  //create new instance
  v8::Local<v8::Object> NewInstance() { return Nan::New(internal->emitter); }

  //emit event
  void Emit(int argc, v8::Local<v8::Value> argv[])
  {
    runInAsyncScope(Nan::New(internal->emitter), "$emit", argc, argv);
    runInAsyncScope(Nan::New(interceptor), "$emit", argc, argv);
  }
};

#endif