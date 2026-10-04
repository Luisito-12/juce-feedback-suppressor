# Guía Rápida: Feedback Suppressor VST3

## Opción 1: Compilar automáticamente con GitHub Actions (5 minutos)

### Paso 1: Crear repositorio en GitHub
1. Ir a https://github.com/new
2. Nombre: `feedback-suppressor` (o similar)
3. Click en "Create repository"

### Paso 2: Subir el código
```bash
# En tu terminal, dentro de la carpeta del proyecto
git init
git add .
git commit -m "Initial commit: JUCE VST3 Feedback Suppressor"
git branch -M main
git remote add origin https://github.com/TU_USUARIO/feedback-suppressor.git
git push -u origin main
```

### Paso 3: Esperar compilación
- GitHub Actions **compilará automáticamente** en ~8-10 minutos
- Ir a: GitHub → Actions (pestaña)
- Verás 1 build: Windows x64
- Cuando termine (✅ verde), ir a "Artifacts"

### Paso 4: Descargar plugin
- Descargar `FeedbackSuppressor-Windows-x64` desde Artifacts
- Extraer el `.zip`
- Copiar carpeta `FeedbackSuppressor.vst3` a:
  - **Windows**: `C:\Program Files\Common Files\VST3\`

### Paso 5: Usar en tu DAW
1. Abre Reaper / Studio One / Cubase (cualquier DAW con soporte VST3)
2. Rescanea plugins
3. Carga "Feedback Suppressor"
4. Aumenta ganancia de micrófono hasta que itere
5. El plugin detecta automáticamente y suprime el feedback ✓

---

## Interfaz Gráfica

Al abrir el plugin ves esto:

```
┌─────────────────────────────────────────┐
│    Feedback Suppressor                  │
├─────────────────────────────────────────┤
│ [X] Enable                              │
├─────────────────────────────────────────┤
│ Threshold (dB):        [====|====-20dB] │
│ Ratio:                 [==|====4.0]     │
│ Q Factor:              [=|=====1.0]     │
│ Max Notches:           [==|====4]       │
│ Output Gain (dB):      [====|====0dB]   │
└─────────────────────────────────────────┘
```

### Ajusta en vivo:
- **Threshold**: Más bajo = más sensible (detecta feedback más fácil)
- **Ratio**: Intensidad de compresión (4:1 suele ser bueno)
- **Q Factor**: Ancho del notch (1 = normal, 10 = muy estrecho)
- **Max Notches**: Cuántos feedback simultáneos suprime (4-8 típico)
- **Output Gain**: Compensa pérdida de volumen

---

## Troubleshooting

### GitHub Actions falla
**Error**: "uses deprecated version `actions/upload-artifact: v3`"
**Solución**: Ir a `.github/workflows/build.yml` y cambiar:
```yaml
# Cambiar esto:
uses: actions/upload-artifact@v3

# Por esto:
uses: actions/upload-artifact@v4
```
Commit y GitHub recompila.

### El plugin no aparece en mi DAW
- Verificar que está en la carpeta correcta
- Rescanear plugins en tu DAW (Settings → Plugin Scan)
- En macOS: Puede necesitar notarización (Mac rechaza plugins sin firma)
- En Linux: Verificar que libros necesarias están instaladas

### Feedback sigue escuchándose
- Aumentar "Max Notches" (permite más notches simultáneos)
- Bajar "Threshold" (detecta feedback más fácil)
- Aumentar "Q Factor" (notch más preciso)

---

## Estructura de carpetas después de descargar

```
feedback-suppressor/
├── CMakeLists.txt          ← Configuración de compilación
├── README.md               ← Documentación completa
├── START_HERE.md           ← Este archivo
├── .github/
│   └── workflows/
│       └── build.yml       ← Compilación automática
└── src/
    ├── PluginProcessor.h/cpp       ← Core DSP
    ├── PluginEditor.h/cpp          ← Interfaz gráfica
    └── DSP/
        ├── FeedbackAnalyzer.h/cpp  ← Detección FFT
        ├── NotchFilter.h/cpp       ← Filtro notch
        └── PeakTracker.h/cpp       ← Seguimiento
```

---

## Próximos pasos (opcional)

Después de que funcione el plugin básico, puedes agregar:

1. **Visualizador espectral** - Ver feedback en tiempo real
2. **Asistente de calibración** - Auto-detectar feedback automáticamente
3. **Reducción de ruido** - Suprime ruido de fondo además de feedback
4. **Detector de voz** - Protege voz mientras suprime ruido

---

## Preguntas frecuentes

**P: ¿Puedo compilar localmente sin GitHub Actions?**
R: Sí, pero necesitas instalar JUCE 7, CMake, compilador. GitHub Actions es más fácil.

**P: ¿Funciona en Audio Units (macOS)?**
R: Sí, el CMakeLists.txt incluye AU. Busca `.component` en lugar de `.vst3`

**P: ¿Latencia del plugin?**
R: ~0 muestras (latencia cero). La detección es en paralelo con el procesamiento.

**P: ¿Cuántos feedback simultáneos suprime?**
R: Hasta 12 notches. Control ajustable en la interfaz.

**P: ¿Funciona en tiempo real?**
R: Sí, adaptativo. Si aparece nuevo feedback, genera nuevo notch automáticamente.

---

**¡Listo!** El plugin está listo para usar en 5 minutos. 🎙️🎚️
