// Add subtle 3d tilt effect to the card on mousemove
const card = document.querySelector('.profile-card');

document.addEventListener('mousemove', (e) => {
    // Custom cursor logic
    const cursorDot = document.querySelector('.cursor-dot');
    const cursorOutline = document.querySelector('.cursor-outline');
    
    if (cursorDot && cursorOutline) {
        cursorDot.style.left = `${e.clientX}px`;
        cursorDot.style.top = `${e.clientY}px`;
        
        // Add a slight delay to the outline for a smooth effect
        cursorOutline.animate({
            left: `${e.clientX}px`,
            top: `${e.clientY}px`
        }, { duration: 150, fill: "forwards" });

        // Expand cursor on clickable elements
        const isHoveringClickable = e.target.closest('a, button, .social-btn');
        if (isHoveringClickable) {
            cursorOutline.style.width = '60px';
            cursorOutline.style.height = '60px';
            cursorOutline.style.backgroundColor = 'rgba(249, 115, 22, 0.1)';
        } else {
            cursorOutline.style.width = '40px';
            cursorOutline.style.height = '40px';
            cursorOutline.style.backgroundColor = 'transparent';
        }
    }

    // Card tilt logic
    if (!card) return;
    
    const xAxis = (window.innerWidth / 2 - e.pageX) / 50;
    const yAxis = (window.innerHeight / 2 - e.pageY) / 50;
    
    // Only apply if we are not hovering over buttons to avoid messy interactions
    const isHoveringBtn = e.target.closest('.social-btn, button');
    if (!isHoveringBtn) {
        card.style.transform = `perspective(1000px) rotateY(${xAxis}deg) rotateX(${yAxis}deg)`;
    } else {
        card.style.transform = `perspective(1000px) rotateY(0deg) rotateX(0deg) translateY(-5px)`;
    }
});

// Reset tilt on mouseleave
document.addEventListener('mouseleave', () => {
    if (!card) return;
    card.style.transform = `perspective(1000px) rotateY(0deg) rotateX(0deg)`;
});

// Theme Toggle Logic
const themeToggle = document.getElementById('theme-toggle');
const body = document.body;
const icon = themeToggle.querySelector('i');

// Check saved theme
const savedTheme = localStorage.getItem('theme');
if (savedTheme === 'dark') {
    body.classList.replace('light-theme', 'dark-theme');
    icon.classList.replace('fa-moon', 'fa-sun');
}

themeToggle.addEventListener('click', () => {
    if (body.classList.contains('light-theme')) {
        body.classList.replace('light-theme', 'dark-theme');
        icon.classList.replace('fa-moon', 'fa-sun');
        localStorage.setItem('theme', 'dark');
    } else {
        body.classList.replace('dark-theme', 'light-theme');
        icon.classList.replace('fa-sun', 'fa-moon');
        localStorage.setItem('theme', 'light');
    }
});



// Live Time Widget (Glasgow Timezone)
const timeDisplay = document.getElementById('local-time');
function updateTime() {
    if (!timeDisplay) return;
    const now = new Date();
    // Use Intl.DateTimeFormat to get time in Glasgow (Europe/London)
    const timeString = new Intl.DateTimeFormat('en-GB', {
        timeZone: 'Europe/London',
        hour: '2-digit',
        minute: '2-digit',
        hour12: false
    }).format(now);
    timeDisplay.textContent = timeString;
}

updateTime();
setInterval(updateTime, 1000);

// Animate links sequentially on load
document.addEventListener('DOMContentLoaded', () => {
    const links = document.querySelectorAll('.social-btn');
    links.forEach((link, index) => {
        link.style.opacity = '0';
        link.style.transform = 'translateY(10px)';
        setTimeout(() => {
            link.style.transition = 'all 0.5s ease';
            link.style.opacity = '1';
            link.style.transform = 'translateY(0)';
        }, 600 + (index * 150));
    });
});
