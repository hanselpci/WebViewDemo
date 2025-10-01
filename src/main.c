#include "webview/webview.h" // ���� webview ��ͷ�ļ�
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
    // Windows ƽ̨��·����ʽΪ file:///C:/path/to/index.html
    char abs_path[MAX_PATH] = {0};
    // ��ȡ����·��
    if (!GetFullPathNameA(file_path, MAX_PATH, abs_path, NULL))
    {
        fprintf(stderr, "��ȡ����·��ʧ�ܣ�%s\n", file_path);
        return NULL;
    }
    // ת��Ϊ file:// ��ʽ����Ҫ�� \ �滻Ϊ /��
    for (int i = 0; abs_path[i]; i++)
    {
        if (abs_path[i] == '\\')
            abs_path[i] = '/';
    }
    // ƴ�� URL��Windows �����̷�ǰ�� /��
    char *url = (char *)malloc(strlen(abs_path) + 8); // "file:///" + ·������
    if (!url)
        return NULL;
    sprintf(url, "file:///%s", abs_path);
    return url;
#else
    // Linux/macOS ƽ̨��·����ʽΪ file:///home/user/path/index.html
    char abs_path[1024] = {0};
    // ��ȡ����·��
    if (!realpath(file_path, abs_path))
    {
        fprintf(stderr, "��ȡ����·��ʧ�ܣ�%s\n", file_path);
        return NULL;
    }
    // ƴ�� URL
    char *url = (char *)malloc(strlen(abs_path) + 8); // "file://" + ·������
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
    // 1. ��ʼ�� webview ����
    webview_t w = webview_create(0, NULL);
    if (!w)
    {
        fprintf(stderr, "Failed to create webview\n");
        return 1;
    }

    // ���ô��ڱ���
    webview_set_title(w, "Webview �ı��鿴��");
    // ���ô��ڴ�С����, ��, �Ƿ�̶���С 0=�ɵ���, 1=�̶���
    webview_set_size(w, 800, 900, 0);
    // ����ȫ�����ɵ��� webview_set_fullscreen(w, 1);

    char *file_url = get_file_url(html_path);
    if (!file_url)
    {
        fprintf(stderr, "�޷������ļ� URL\n");
        return 1;
    }
    printf("���ر����ļ���%s\n", file_url);

    // 2. ���ر��� HTML �ļ���·�����������ִ���ļ����Ӧ��
    webview_navigate(w, file_url);

    // 3. ���� webview �¼�ѭ����������ֱ�����ڹرգ�
    webview_run(w);

    // 4. �ͷ� webview ��Դ
    webview_destroy(w);
    return 0;
}