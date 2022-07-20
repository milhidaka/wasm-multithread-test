const express = require('express');
const app = express();
const PORT = 8080;

app.use(express.static('public', {
  setHeaders: function (res, path) {
    // no allow cache
    res.set("Cache-Control", "no-cache");
  
    // CORS
    res.set("Access-Control-Allow-Origin", "*");
    res.set("Access-Control-Allow-Methods", "GET,POST,HEAD,OPTIONS");
  
    // COEP
    res.set("Cross-Origin-Embedder-Policy", "require-corp");
    res.set("Cross-Origin-Opener-Policy", "same-origin");
  }
}));

app.listen(PORT, () => console.log(`Server listening on port: ${PORT}`));
