import sys
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

# Receber argumentos da linha de comando
# Formato: python plot_vectors.py Ax Ay Az Bx By Bz [Cx Cy Cz]
if len(sys.argv) < 7:
    print("Uso: python plot_vectors.py Ax Ay Az Bx By Bz [Cx Cy Cz]")
    sys.exit(1)

# Ler coordenadas dos vetores
Ax, Ay, Az = int(sys.argv[1]), int(sys.argv[2]), int(sys.argv[3])
Bx, By, Bz = int(sys.argv[4]), int(sys.argv[5]), int(sys.argv[6])

# Criar figura 3D
fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111, projection='3d')

# Desenhar eixos
ax.plot([0, 10], [0, 0], [0, 0], 'r-', linewidth=2, label='Eixo X')
ax.plot([0, 0], [0, 10], [0, 0], 'g-', linewidth=2, label='Eixo Y')
ax.plot([0, 0], [0, 0], [0, 10], 'b-', linewidth=2, label='Eixo Z')

# Desenhar vetor A (amarelo)
ax.quiver(0, 0, 0, Ax, Ay, Az, color='yellow', arrow_length_ratio=0.2, linewidth=3, label=f'Vetor A ({Ax}, {Ay}, {Az})')

# Desenhar vetor B (ciano)
ax.quiver(0, 0, 0, Bx, By, Bz, color='cyan', arrow_length_ratio=0.2, linewidth=3, label=f'Vetor B ({Bx}, {By}, {Bz})')

# Desenhar vetor C se fornecido (magenta)
if len(sys.argv) >= 10:
    Cx, Cy, Cz = int(sys.argv[7]), int(sys.argv[8]), int(sys.argv[9])
    ax.quiver(0, 0, 0, Cx, Cy, Cz, color='magenta', arrow_length_ratio=0.2, linewidth=3, label=f'Vetor C ({Cx}, {Cy}, {Cz})')

# Configurar limites dos eixos
max_val = max(abs(Ax), abs(Ay), abs(Az), abs(Bx), abs(By), abs(Bz))
if len(sys.argv) >= 10:
    max_val = max(max_val, abs(Cx), abs(Cy), abs(Cz))
max_val = max(max_val, 5)

ax.set_xlim([-max_val-1, max_val+1])
ax.set_ylim([-max_val-1, max_val+1])
ax.set_zlim([-max_val-1, max_val+1])

# Adicionar grade
ax.grid(True, alpha=0.3)

# Adicionar labels
ax.set_xlabel('X', fontsize=12)
ax.set_ylabel('Y', fontsize=12)
ax.set_zlabel('Z', fontsize=12)
ax.set_title('Visualização 3D dos Vetores', fontsize=14, fontweight='bold')

# Adicionar legenda
ax.legend(loc='upper left')

# Mostrar gráfico
plt.tight_layout()
plt.show()

