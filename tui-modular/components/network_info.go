package components

import (
	"strings"

	"github.com/charmbracelet/lipgloss"
)

type NetworkInfoBox struct {
	name          string
	dw_speed      float64
	dw_top        float64
	dw_total      float64
	up_speed      float64
	up_top        float64
	up_total      float64
	width, height int
}

func (n *NetworkInfoBox) Update(stats NetStats) {
	if n.name == stats.Name {
		n.dw_speed = stats.download_speed
		n.up_speed = stats.upload_speed
		n.dw_top = stats.download_top
		n.up_top = stats.upload_top
		n.dw_total = stats.download_total
		n.up_total = stats.upload_total
	}
}

func (n *NetworkInfoBox) SetSize(w, h int) {
	n.height = h
	n.width = w
}

func NewNetworkInfoBox() *NetworkInfoBox {
	return &NetworkInfoBox{}
}

func (n *NetworkInfoBox) View() string {
	blockStyle := lipgloss.NewStyle().
		Width(n.width).
		Height(n.height).
		Background(lipgloss.Color("#00AAFF")) // Light blue

	content := strings.Repeat(" ", n.width*n.height)

	return blockStyle.Render(content)
}
