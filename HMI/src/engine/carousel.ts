export default class Carousel {
    constructor() {
        const carouselItems = document.getElementsByClassName('carousel-item');
        for (let i = 0; i < carouselItems.length; i++) {
            carouselItems[i].addEventListener('click', () => {
                for (let j = 0; j < carouselItems.length; j++) {
                    if (carouselItems[i] === carouselItems[j]) {
                        carouselItems[j].classList.toggle('select');
                    } else {
                        carouselItems[j].classList.remove('select');
                    }
                }
            });
        }
    }

    update() {
    }
}