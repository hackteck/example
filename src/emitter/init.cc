#include <nan.h>
#include "./emitter.cc"
EventEmitter *emitter = new EventEmitter();

NAN_METHOD(emitTestEvent)
{
  v8::Local<v8::Value> argv[] = {
      Nan::New("foo").ToLocalChecked(),
      Nan::New("bar").ToLocalChecked()
  };
  emitter->Emit(2, argv);
  info.GetReturnValue().Set(emitter->NewInstance());
}

NAN_MODULE_INIT(Init)
{
  Nan::Set(target, Nan::New("obj").ToLocalChecked(), emitter->NewInstance());
  NAN_EXPORT(target, emitTestEvent);
}
NODE_MODULE(Example, Init);