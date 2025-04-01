
class PopUpContent {
    content:HTMLElement|undefined = undefined;
    timeout:number|undefined = undefined;
}
let contents:PopUpContent[] = [];

export function openPopupInfo(message:string) {
    let id:string = Math.floor(Math.random() * 100000000).toString();
    openPopup(message, id);
    document.getElementById(id)!.style.backgroundColor = '#68b33c';
}

export function openPopupWarning(message:string) {
    let id:string = Math.floor(Math.random() * 100000000).toString();
    openPopup(message, id);
    document.getElementById(id)!.style.backgroundColor = '#b3993c';
}
export function openPopupError(message:string) {
    let id:string = Math.floor(Math.random() * 100000000).toString();
    openPopup(message, id);
    document.getElementById(id)!.style.backgroundColor = '#b33c40';
}
function openPopup(message:string, id:string) {
    const popMessage= document.createElement("span");
    popMessage.classList.add("popup-message");
    popMessage.innerText = message;

    const close = document.createElement("a");
    close.classList.add("popup-close");
    close.innerText = '✖️';
    close.addEventListener('click', () => {
        content.style.display = 'none';
        let popMessage = document.getElementById(id);
        if(popMessage) {
            document.getElementById('popup')?.removeChild(popMessage);
            if(document.getElementById('popup')?.children.length == 0) {
                document.getElementById('popup')!.style.display = 'none';
            }
        }
    });

    const content = document.createElement("div");
    content.classList.add("popup-content");
    content.id = id;
    content.append(popMessage);
    content.append(close);
    contents.push({content, timeout: setTimeout(() => {
        if(document.getElementById('popup')!.hasChildNodes()) {
            document.getElementById('popup')!.removeChild(content);
            document.getElementById('popup')!.style.display = 'none';
            close.removeEventListener('click', () => {});
            }
        }, 5000)});

    document.getElementById('popup')!.style.display = 'block';
    document.getElementById('popup')!.appendChild(content);

}