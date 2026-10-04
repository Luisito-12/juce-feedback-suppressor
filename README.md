# Feedback Suppressor VST3 Plugin

Plugin VST3 de supresión automática de realimentación (feedback) para DAWs usando JUCE 7 y C++.

## Características

- **Detección automática de feedback**: FFT en tiempo real (2048 samples, latencia cero)
- **Filtros notch adaptativos**: Hasta 12 notches simultáneos
- **Interfaz gráfica intuitiva**: Sliders para ajustar en vivo
  - Threshold de detección (-60 a 0 dB)
  - Ratio de compresión (1:1 a 10:1)
  - Factor Q de los notches (0.5 a 10)
  - Cantidad máxima de notches (1 a 12)
  - Ganancia de salida (-12 a 12 dB)

## Compilación con GitHub Actions (Windows)

1. **Crear repositorio GitHub** (si no existe):
   ```bash
   git init
   git add .
   git commit -m "Initial commit"
   git branch -M main
   git remote add origin https://github.com/USERNAME/feedback-suppressor.git
   git push -u origin main
   ```

2. **Subir este proyecto** a tu repositorio

3. **GitHub Actions compilará automáticamente** para:
   - Windows x64 (MSVC)
   - Tiempo: ~8-10 minutos

4. **Descargar compilado** desde Actions → Artifacts

## Instalación de plugin compilado

### Windows x64
```
C:\Program Files\Common Files\VST3\FeedbackSuppressor.vst3
```

Si la carpeta no existe, crearla manualmente. El plugin es de 64 bits.

## Uso

1. Abre tu DAW (Reaper, Studio One, Cubase, etc.)
2. Carga el plugin como VST3
3. Aumenta el nivel de micrófono hasta que comience el feedback
4. El plugin detectará y suprimirá automáticamente las frecuencias de feedback

### Controles

| Control | Rango | Por defecto | Descripción |
|---------|-------|-------------|-------------|
| Enable | On/Off | On | Activar/desactivar plugin |
| Threshold | -60...0 dB | -20 dB | Nivel mínimo para detectar feedback |
| Ratio | 1...10 | 4 | Relación de compresión |
| Q Factor | 0.5...10 | 1 | Ancho de banda del notch (mayor Q = más estrecho) |
| Max Notches | 1...12 | 4 | Cantidad máxima de notches simultáneos |
| Output Gain | -12...12 dB | 0 dB | Compensación de ganancia |

## Requisitos de compilación (local Windows)

- Visual Studio 2019+ (con MSVC)
- CMake 3.21+
- C++17 (MSVC)
- JUCE 7.0.9 (descargado automáticamente)

## Estructura del proyecto

```
juce-feedback-suppressor/
├── CMakeLists.txt                 # Configuración CMake
├── README.md                       # Este archivo
├── .github/
│   └── workflows/
│       └── build.yml               # GitHub Actions workflow
└── src/
    ├── PluginProcessor.h/cpp       # Procesador de audio principal
    ├── PluginEditor.h/cpp          # Interfaz gráfica
    └── DSP/
        ├── FeedbackAnalyzer.h/cpp  # Análisis FFT
        ├── NotchFilter.h/cpp       # Filtro notch biquad
        └── PeakTracker.h/cpp       # Seguimiento de picos
```

## Parámetros de procesamiento DSP

### FFT
- Tamaño: 2048 samples
- Ventana: Hann
- Resolución: ~21 Hz @ 44.1 kHz

### Filtro Notch
- Tipo: Biquad IIR (Direct Form II)
- Latencia: 0 samples
- Verificación de persistencia: 3 bloques mínimo

### Detección de Feedback
- Detección de picos locales en el espectro
- Seguimiento temporal para evitar falsos positivos
- Atenuación variable (hasta -6 dB por notch)

## Troubleshooting

### GitHub Actions falla
- Revisar que `.github/workflows/build.yml` tenga `upload-artifact@v4`
- Verificar que el repositorio tiene acceso a GitHub Actions habilitado
- El build debe durar 8-10 minutos máximo

### El plugin no se carga en DAW
- Verificar que está en `C:\Program Files\Common Files\VST3\`
- Rescan plugins en tu DAW
- Comprobar que es x64 (no x86 de 32 bits)
- Reiniciar el DAW completamente

### DAW se congela
- Esperar 5-10 segundos en la primera carga
- Verificar que no hay conflictos con otros plugins VST3

## Plataformas soportadas

- **Windows** x64 (Visual Studio MSVC)
- Solo VST3 (no VST2, AAX, AU)

## Próximas fases (opcional)

1. **Visualizador espectral** - Mostrar feedback detectado en tiempo real
2. **Asistente de calibración** - Auto-detectar feedback automáticamente
3. **Detector de voz** - Proteger voz mientras suprime ruido
4. **Reducción de ruido adaptativo** - EQ por bandas

## Licencia

Desarrollado como herramienta de ingeniería de sonido. Libre para uso personal.

## Soporte

Para problemas o sugerencias, crear un issue en GitHub o contactar al desarrollador.
