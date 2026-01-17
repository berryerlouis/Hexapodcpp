import { defineConfig } from 'vite'

export default defineConfig({ 
    build: { sourcemap: true },
    server: {
        hmr: true,
        watch: {
            usePolling: true,
        },
    },
})
