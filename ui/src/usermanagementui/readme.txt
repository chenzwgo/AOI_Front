建议翻译说明
1. 使用lupdate 命令，lupdate 工具将创建，生成翻译文件（后缀名为.ts）
    例如 lupdate ui.pro 
    根据ui.pro 中的描述
    TRANSLATIONS += en_us.ts \
               cn_zh.ts  
    命令，lupdate 将创建 en_us.ts ， cn_zh.ts 这两个文件
2. 使用Qt linguist 工具分别打开这两个文件，进行翻译。运行发布命令 将生成后缀名为(.qm)的翻译文件

3. 代码中调用 .qm 文件进行翻译
