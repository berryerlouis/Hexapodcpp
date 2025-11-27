class PopUpContent {
    content: HTMLElement | undefined = undefined;
    timeout: number | undefined = undefined;
}

let contents: PopUpContent[] = [];

export function clearAllPopups() {
    // Clear all timeouts
    contents.forEach(popupContent => {
        if (popupContent.timeout) {
            clearTimeout(popupContent.timeout);
        }
    });
    contents = [];

    // Clear DOM
    const popup = document.getElementById('popup');
    if (popup) {
        popup.innerHTML = '';
        popup.style.display = 'none';
    }
}

export function openPopupInfo(message: string) {
    let id: string = Math.floor(Math.random() * 100000000).toString();
    openPopup(message, id);
    document.getElementById(id)!.style.backgroundColor = '#68b33c';
}

export function openPopupWarning(message: string) {
    let id: string = Math.floor(Math.random() * 100000000).toString();
    openPopup(message, id);
    document.getElementById(id)!.style.backgroundColor = '#b3993c';
}

export function openPopupError(message: string) {
    let id: string = Math.floor(Math.random() * 100000000).toString();
    openPopup(message, id);
    document.getElementById(id)!.style.backgroundColor = '#b33c40';
}

function openPopup(message: string, id: string) {
    const popMessage = document.createElement("span");
    popMessage.classList.add("popup-message");
    popMessage.innerText = message;

    const close = document.createElement("a");
    close.classList.add("popup-close");
    close.innerText = '✖️';

    const closeHandler = () => {
        content.style.display = 'none';
        let popMessage = document.getElementById(id);
        if (popMessage) {
            document.getElementById('popup')?.removeChild(popMessage);
            if (document.getElementById('popup')?.children.length == 0) {
                document.getElementById('popup')!.style.display = 'none';
            }
        }

        // Clean up timeout and reference
        const contentIndex = contents.findIndex(c => c.content?.id === id);
        if (contentIndex !== -1) {
            const popupContent = contents[contentIndex];
            if (popupContent.timeout) {
                clearTimeout(popupContent.timeout);
            }
            contents.splice(contentIndex, 1);
        }
        close.removeEventListener('click', closeHandler);
    };

    close.addEventListener('click', closeHandler);

    const content = document.createElement("div");
    content.classList.add("popup-content");
    content.id = id;
    content.append(popMessage);
    content.append(close);

    const timeout = setTimeout(() => {
        const popup = document.getElementById('popup');
        if (popup?.hasChildNodes() && content.parentElement) {
            popup.removeChild(content);
            if (popup.children.length === 0) {
                popup.style.display = 'none';
            }
        }

        const contentIndex = contents.findIndex(c => c.content?.id === id);
        if (contentIndex !== -1) {
            contents.splice(contentIndex, 1);
        }
        close.removeEventListener('click', closeHandler);
    }, 5000);

    contents.push({ content, timeout });

    document.getElementById('popup')!.style.display = 'block';
    document.getElementById('popup')!.appendChild(content);
}