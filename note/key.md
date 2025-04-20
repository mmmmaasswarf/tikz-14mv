# 关于键值对

## 概述

键的名称不展开.

### 在包中的用法

创建`setup`函数

```tex
\DeclareDocumentCommand \MyModuleSetup { m }
{ \keys_set:nn { mymodule } { #1 } }
```

作为某个特定函数的可选项

```tex
 \DeclareDocumentCommand \MyModuleMacro { o m }
 {
 \group_begin: %限定作用域
 \keys_set:nn { mymodule } { #1 }
 % Main code for \MyModuleMacro
 \group_end:
 }
```
