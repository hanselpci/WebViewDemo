#include "webview/webview.h" // 引入 webview 库头文件
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>

char *getExecutablePath()
{
    static char path[MAX_PATH];

    GetModuleFileNameA(NULL, path, MAX_PATH);

    char *pos = strrchr(path, '\\');
    if (pos != NULL)
    {
        *pos = '\0';
    }

    return path;
}

char *getCurrentPath()
{
    static char path[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, path);
    return path;
}

char *get_file_url(const char *file_path)
{
    if (!file_path)
        return NULL;

#ifdef _WIN32
    // Windows 平台：路径格式为 file:///C:/path/to/index.html
    char abs_path[MAX_PATH] = {0};
    // 获取绝对路径
    if (!GetFullPathNameA(file_path, MAX_PATH, abs_path, NULL))
    {
        fprintf(stderr, "获取绝对路径失败：%s\n", file_path);
        return NULL;
    }
    // 转换为 file:// 格式（需要将 \ 替换为 /）
    for (int i = 0; abs_path[i]; i++)
    {
        if (abs_path[i] == '\\')
            abs_path[i] = '/';
    }
    // 拼接 URL（Windows 需在盘符前加 /）
    char *url = (char *)malloc(strlen(abs_path) + 8); // "file:///" + 路径长度
    if (!url)
        return NULL;
    sprintf(url, "file:///%s", abs_path);
    return url;
#else
    // Linux/macOS 平台：路径格式为 file:///home/user/path/index.html
    char abs_path[1024] = {0};
    // 获取绝对路径
    if (!realpath(file_path, abs_path))
    {
        fprintf(stderr, "获取绝对路径失败：%s\n", file_path);
        return NULL;
    }
    // 拼接 URL
    char *url = (char *)malloc(strlen(abs_path) + 8); // "file://" + 路径长度
    if (!url)
        return NULL;
    sprintf(url, "file://%s", abs_path);
    return url;
#endif
}

int main()
{
    const char *html_path = "web/index.html";

    printf("Executable Path: %s\n", getExecutablePath());
    printf("Current Path: %s\n", getCurrentPath());
    // 1. 初始化 webview 窗口
    webview_t w = webview_create(0, NULL);
    if (!w)
    {
        fprintf(stderr, "Failed to create webview\n");
        return 1;
    }

    // 设置窗口标题
    webview_set_title(w, "Webview 文本查看器");
    // 设置窗口大小（宽, 高, 是否固定大小 0=可调整, 1=固定）
    webview_set_size(w, 800, 900, 0);
    // 如需全屏，可调用 webview_set_fullscreen(w, 1);

    char *file_url = get_file_url(html_path);
    if (!file_url)
    {
        fprintf(stderr, "无法生成文件 URL\n");
        return 1;
    }
    printf("加载本地文件：%s\n", file_url);

    // 2. 加载本地 HTML 文件（路径需与编译后可执行文件相对应）
    webview_navigate(w, file_url);

    // 3. 启动 webview 事件循环（阻塞，直到窗口关闭）
    webview_run(w);

    // 4. 释放 webview 资源
    webview_destroy(w);
    return 0;
}