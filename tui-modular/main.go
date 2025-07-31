package main

import (
	"fmt"
	"os"
	"tui-modular/components"

	tea "github.com/charmbracelet/bubbletea"
	"github.com/charmbracelet/lipgloss"
)

var check string

type model struct {
	width     int
	height    int
	cpuBox    *components.CPUBox
	disksBox  *components.DisksBox
	memoryBox *components.MemoryBox
}

func (m *model) layoutComponents() {
	cpuHeight := m.height / 3
	diskHeight := m.height / 3
	memHeight := m.height / 3
	//procHeight := (2 * m.height) / 3
	//netHeight := m.height / 3

	cpuWidth := m.width - 4
	diskWidth := m.width / 4
	memWidth := m.width / 4
	//procWidth := m.width - memWidth - diskWidth
	//netWidth := m.width - memWidth

	m.cpuBox.SetSize(cpuWidth, cpuHeight)
	m.disksBox.SetSize(diskWidth, diskHeight)
	m.memoryBox.SetSize(memWidth, memHeight)
	//m.memBox.SetSize(memWidth, memHeight)
	//m.netBox.SetSize(netWidth, netHeight)
	//m.procBox.SetSize(procWidth, procHeight)
}

func (m model) Init() tea.Cmd {
	return nil
}

func (m model) Update(msg tea.Msg) (tea.Model, tea.Cmd) {
	switch msg := msg.(type) {
	case tea.KeyMsg:
		if msg.String() == "q" || msg.String() == "ctrl+c" {
			return m, tea.Quit
		}
	case tea.WindowSizeMsg:
		m.width = msg.Width
		m.height = msg.Height
		m.layoutComponents()
		// m.cpuBox.SetSize(m.width, cpuHeight)
	}
	return m, nil
}

func (m model) View() string {
	return lipgloss.JoinVertical(lipgloss.Top, m.cpuBox.View(), lipgloss.JoinHorizontal(lipgloss.Left, m.memoryBox.View(), m.disksBox.View()))

	// how to join different modular components
	// 	return lipgloss.JoinVertical(lipgloss.Left, m.cpuBox.View(),
	// lipgloss.JoinHorizontal(lipgloss.Left, m.memBox.View(), m.procBox.View()))
}

func main() {
	check = "aa"
	p := tea.NewProgram(model{
		cpuBox:    components.NewCPUBox(),
		disksBox:  components.NewDisksBox(),
		memoryBox: components.NewMemoryBox(),
	}, tea.WithAltScreen())

	if _, err := p.Run(); err != nil {
		fmt.Println("Error running program: ", err)
		os.Exit(1)
	}
}
