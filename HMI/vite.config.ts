import { defineConfig } from 'vite'

export default defineConfig({
    build: {
        sourcemap: true,
        chunkSizeWarningLimit: 700,
        rollupOptions: {
            output: {
                manualChunks(id) {
                    if (!id.includes('node_modules')) {
                        return undefined;
                    }

                    if (id.includes('three')) {
                        return 'vendor-three';
                    }

                    return 'vendor';
                },
            },
        },
    },
    server: {
        hmr: true,
        watch: {
            usePolling: true,
        },
    },
})
