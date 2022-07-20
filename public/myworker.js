if (typeof globalThis.Module === "undefined") {
  globalThis.Module = {};
  globalThis.Module.onRuntimeInitialized = () => {
    const m = 1024, n = 1024, k = 1024;
    const parallel = 4;
    console.log("m,n,k", m, n, k);
    console.log("parallel=", parallel);
    Module._do_alloc(m, n, k);
    console.time("sgemm");
    Module._compute(m, n, k, parallel);
    console.timeEnd("sgemm");
  };
    
  importScripts("./main.js");
  
  console.log("Module", globalThis.Module);
} else {
  console.log("inside sub-worker");
  importScripts("./main.js");
}
