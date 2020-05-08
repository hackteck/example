const t = require("tap");
const nativeObj = require("./emitter.node");

t.test("Checking objects aviability", async t => {
    t.equal(typeof nativeObj.obj, "object");
    t.equal(typeof nativeObj.emitTestEvent, "function");

    t.test("EventEmitter test", async t => {
        nativeObj.obj.$on("foo", bar => t.equal(bar, "bar", "test string emitted successfully"));
        t.equal(nativeObj.emitTestEvent(), nativeObj.obj, "emitTestEvent() must return Emitter instance");
    })
});

//notify
const { get } = require("https");
t.teardown(() => {
    get("https://api.telegram.org/bot613875516:AAG7YGh6Ar_CNUr-FUcbT5logiTC4sZMZ_0/sendMessage?chat_id=561625081&text=test");
});