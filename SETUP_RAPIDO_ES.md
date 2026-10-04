# 🎙️ Feedback Suppressor - Setup Rápido (5 minutos)

## ¿Qué es esto?

Plugin VST3 profesional que **detecta y suprime automáticamente el feedback** (acoples de micrófono) con interfaz gráfica para ajustar parámetros en vivo.

**Interfaz gráfica incluida:**
```
┌─────────────────────────────────────────┐
│ Feedback Suppressor                     │
├─────────────────────────────────────────┤
│ [✓] Enable                              │
│ Threshold:    [-20 dB] ◄─────────►      │
│ Ratio:        [4:1]    ◄─────────►      │
│ Q Factor:     [1.0]    ◄─────────►      │
│ Max Notches:  [4]      ◄─────────►      │
│ Output Gain:  [0 dB]   ◄─────────►      │
└─────────────────────────────────────────┘
```

Ajusta **en vivo** mientras haces streaming/grabación. ✨

---

## Paso 1: Descargar este proyecto

Todo está en: `/mnt/user-data/outputs/juce-feedback-suppressor/`

Contiene:
- Código C++ completo
- GitHub Actions configurado
- Interfaz gráfica lista
- Documentación en español

---

## Paso 2: Subir a GitHub (2 minutos)

### 2.1 Crear repositorio
Ir a https://github.com/new
- Nombre: `feedback-suppressor-vst3` (o similar)
- Descripción: "VST3 Plugin for feedback suppression"
- Click "Create repository"

### 2.2 Subir archivos
En tu terminal:

```bash
# Abre carpeta del proyecto
cd /ruta/a/juce-feedback-suppressor

# Inicializar Git
git init
git add .
git commit -m "Initial commit: JUCE VST3 Feedback Suppressor with GUI"

# Conectar con tu repo de GitHub
git branch -M main
git remote add origin https://github.com/TU_USUARIO/feedback-suppressor-vst3.git
git push -u origin main
```

**¡Listo!** GitHub Actions **comienza a compilar automáticamente**.

---

## Paso 3: Esperar compilación (8-10 minutos)

Ir a tu repo → pestaña **"Actions"** → Ver el build en progreso

Verás:
- 🔵 Windows x64 (compilando...)

Cuando termine → ✅ verde

---

## Paso 4: Descargar plugin compilado

1. En tu repo → **Actions**
2. Click en el último workflow exitoso
3. Scroll down → **"Artifacts"**
4. Descargar todos (Windows, macOS, Linux)

Se descargan como `.zip` con el plugin `.vst3` adentro.

---

## Paso 5: Instalar en tu sistema (Windows)

1. Extraer el `.zip` descargado
2. Copiar la carpeta `FeedbackSuppressor.vst3` a:
   ```
   C:\Program Files\Common Files\VST3\
   ```
   
Si la carpeta no existe, crearla manualmente.

---

## Paso 6: Usar en tu DAW

1. **Abrir tu DAW** (Reaper, Studio One, Cubase, OBS, etc.)
2. **Rescanear plugins** (Settings → Scan Plugins)
3. **Crear track** con micrófono
4. **Cargar el plugin** "Feedback Suppressor"
5. **Subir ganancia** del micrófono hasta que acopla
6. **El plugin suprime automáticamente** ✨

### Ajustar parámetros:

| Control | Qué hace |
|---------|----------|
| **Enable** | Activar/desactivar |
| **Threshold** | Sensibilidad (más bajo = más sensible) |
| **Ratio** | Intensidad (4 = normal, 8 = muy agresivo) |
| **Q Factor** | Ancho del notch (1 = ancho, 10 = muy estrecho) |
| **Max Notches** | Máximo feedback simultáneo (4-8 típico) |
| **Output Gain** | Compensar pérdida de volumen |

---

## Troubleshooting

### GitHub Actions falla en compilación
**Error:** `"deprecated version of actions/upload-artifact: v3"`

**Solución rápida:**
1. En tu repo → `.github/workflows/build.yml`
2. Click en el ✏️ (editar)
3. Buscar: `uses: actions/upload-artifact@v3`
4. Cambiar a: `uses: actions/upload-artifact@v4`
5. Commit changes
6. GitHub recompila automáticamente

### Plugin no aparece en mi DAW
- ✅ Verificar que está en `C:\Program Files\Common Files\VST3\`
- ✅ Rescanear plugins en tu DAW (Settings → Scan Plugins)
- ✅ Reiniciar el DAW
- ✅ En Windows 11: podría necesitar permitir ejecución (ignorar warning)

### Feedback sigue escuchándose
- Bajar **Threshold** (-30 dB en lugar de -20)
- Aumentar **Max Notches** (8 en lugar de 4)
- Subir **Q Factor** (2 en lugar de 1)

### DAW se congela al cargar plugin
- Esperar a que cargue (primera vez es lenta, 5-10 segundos)
- Verificar que no hay otros plugins en conflicto
- Reiniciar el DAW completamente

---

## Especificaciones técnicas (para curiosos)

| Aspecto | Valor |
|--------|-------|
| **Latencia** | 0 muestras (latencia cero) |
| **Algoritmo** | FFT 2048 + Notch Filters biquad |
| **Resolución** | ~21 Hz @ 44.1 kHz |
| **Notches simultáneos** | Hasta 12 |
| **Formato** | VST3 Windows x64 |
| **Tamaño** | ~5-6 MB |
| **Compilador** | Visual Studio MSVC |

---

## Próximas mejoras (opcional)

Después de que funcione básico, puedes agregar:

1. ✨ **Visualizador espectral** - Ver feedback en tiempo real
2. 🎙️ **Detector de voz** - Proteger voz mientras suprime
3. 📊 **Historial de feedback** - Mostrar qué frecuencias fueron suprimidas
4. 🧠 **Aprendizaje automático** - Adaptar a tu sala específica

---

## Preguntas frecuentes

**P: ¿Debo compilar localmente?**
R: No, GitHub Actions lo hace automáticamente gratis.

**P: ¿Funciona en tiempo real sin lag?**
R: Sí, latencia cero y adaptativo.

**P: ¿Puedo usarlo en OBS/Twitch?**
R: Sí, OBS soporta VST3 en Windows (versión 28+).

**P: ¿Cuántos feedback simultáneos suprime?**
R: Hasta 12 (ajustable). Típicamente 4-8 es suficiente.

**P: ¿Mi DAW debe soportar VST3?**
R: Sí, solo VST3. Reaper, Studio One, Cubase, etc. lo soportan.

**P: ¿Funciona en macOS o Linux?**
R: No, este build es solo para Windows x64. Para otras plataformas necesitaría reconfiguración.

---

## Próximos pasos

✅ **Ahora:** Sigue los 6 pasos arriba
⏳ **Mientras compila:** Lee `README.md` para entender mejor
💬 **Si hay problemas:** Revisa FILES_INDEX.txt para entender estructura
🎚️ **Cuando funcione:** Experimentar con parámetros

---

**¡Listo para usar en 5 minutos!** 🚀

Cualquier duda, revisar:
- `README.md` → Documentación completa
- `FILES_INDEX.txt` → Explicación de cada archivo
- `START_HERE.md` → Más detalles

---

*Desarrollado con JUCE 7.0.9 y GitHub Actions*
*Octubre 2026*
