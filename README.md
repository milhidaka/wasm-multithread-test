webworker上でWASMのマルチスレッドを動かす実験コード

- COEP対応のHTTPサーバ
- emscriptenで生成したマルチスレッドWASMをworker上で正しく動かす

依存パッケージ

```
npm install
```

ビルド(emscripten必要)

```
npm run build
```

実行

```
npm run serve
```

[http://localhost:8080/](http://localhost:8080/) を開く
