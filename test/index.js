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