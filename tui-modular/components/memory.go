package components

import (
	"fmt"
	"math/rand"
	"strings"

	"github.com/charmbracelet/lipgloss"
)

type MemoryBox struct {
	width     int
	height    int
	total     float64
	used      float64
	free      float64
	cached    float64
	available float64
}

func (m *MemoryBox) SetSize(w, h int) {
	m.width = w
	m.height = h
}

func NewMemoryBox() *MemoryBox {
	randomData := randomizedMemData()
	return &MemoryBox{
		total:     randomData.total,
		free:      randomData.free,
		used:      randomData.used,
		cached:    randomData.cached,
		available: randomData.available,
	}
}

func (m *MemoryBox) View() string {
	var usageStr string
	gibStr := fmt.Sprintf("%.2f GiB", m.total)
	spacing := m.width - len("Total:") - len(gibStr) - 4
	if spacing < 0 {
		spacing = 0
	}
	newLine := strings.Repeat("-", m.width)
	usageStr = fmt.Sprintf("Total:%s%s\n", strings.Repeat(" ", spacing+1), gibStr) + newLine
	usageStr += fmt.Sprintf("Used:                         %.2f GiB\n\n\n", m.used) + newLine
	usageStr += fmt.Sprintf("Available:                    %.2f GiB\n\n\n", m.available) + newLine
	usageStr += fmt.Sprintf("Cached:                       %.2f GiB\n\n\n", m.cached) + newLine
	usageStr += fmt.Sprintf("Free:                         %.2f GiB\n", m.free)

	boxStyle := lipgloss.NewStyle().Width(m.width).
		Height(m.height).
		Border(lipgloss.NormalBorder()).
		Align(lipgloss.Center).
		Padding(0, 0)

	return boxStyle.Render(usageStr)
}

func randomizedMemData() MemoryBox {
	var output MemoryBox
	output.total = 8.00
	output.used = rand.Float64() * 8.00
	output.cached = rand.Float64() * output.used
	output.available = output.total - output.used
	output.free = rand.Float64() * 1.00

	return output
}
