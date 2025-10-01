// 绑定按钮与文件选择器（点击按钮触发文件选择弹窗）
const selectBtn = document.getElementById('selectBtn');
const fileSelector = document.getElementById('fileSelector');
const contentArea = document.getElementById('contentArea');

selectBtn.addEventListener('click', () => {
    fileSelector.click();
});

// 选择文件后读取内容并显示
fileSelector.addEventListener('change', async (e) => {
    const file = e.target.files[0];
    if (!file) return; // 未选择文件则退出

    try {
        // 1. 读取本地文本文件（FileReader 原生 API）
        const reader = new FileReader();
        reader.onload = (event) => {
            // 2. 清空原有内容（移除“无文件”提示）
            contentArea.innerHTML = '';
            // 3. 创建 pre 标签显示文本（保留换行/空格格式）
            const pre = document.createElement('pre');
            pre.textContent = event.target.result;
            contentArea.appendChild(pre);
        };
        // 以 UTF-8 编码读取文本（适配中文等多语言）
        reader.readAsText(file, 'UTF-8');
    } catch (err) {
        // 异常处理（如文件无法读取）
        contentArea.innerHTML = `<p class="empty-tip" style="color: red;">文件读取失败：${err.message}</p>`;
    }
});