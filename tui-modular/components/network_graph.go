package components

import (
	"github.com/charmbracelet/lipgloss"

	"math"
	"strings"
)

type NetworkGraphBox struct {
	name          string
	width, height int
	maxPoint      float64
	uploadHist    []float64
	downloadHist  []float64
}

func (n *NetworkGraphBox) SetSize(w, h int) {
	n.height = h
	n.width = w
}

func (n *NetworkGraphBox) Update(stats NetStats) {
	if n.name == stats.Name {
		n.downloadHist = append(n.downloadHist, stats.download_speed)
		n.uploadHist = append(n.uploadHist, stats.upload_speed)
		n.maxPoint = math.Max(stats.download_top, n.maxPoint)
	}
}

func NewNetworkGraphBox() *NetworkGraphBox {
	return &NetworkGraphBox{}
}

func (n *NetworkGraphBox) View() string {
	blockStyle := lipgloss.NewStyle().
		Width(n.width).
		Height(n.height).
		Background(lipgloss.Color("#00AAFF")) // Light blue

	content := strings.Repeat(" ", n.width*n.height)

	return blockStyle.Render(content)
}
