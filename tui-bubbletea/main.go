package main

import (
	"fmt"
	"log"
	"math/rand"
	"os"
	"strings"
	"time"

	"github.com/charmbracelet/bubbles/progress"
	tea "github.com/charmbracelet/bubbletea"
	"github.com/charmbracelet/lipgloss"
)

const (
	padding  = 2
	maxWidth = 80
)

var helpStyle = lipgloss.NewStyle().Foreground(lipgloss.Color("#626262")).Render
var mainStyle = lipgloss.NewStyle().MarginLeft(1)

type tuiModel struct {
	header   topHeaderModel
	deets    []daemonMsg
	progress []progress.Model
}

type topHeaderModel struct {
	topCoreCpu    float64
	coresCpuUsage []float64
	uptime        float64
	memory        float64
	swap          float64
}

// metrics data that we would receive from presentation/query-layer C program
type daemonMsg struct {
	process_id           int
	parent_pid           int
	process_name         string
	process_state        string
	vm_size              float64 // in kb
	vm_res_mem           float64 // in kb
	cpuUsage             int     // current cpu percentage being used by this process
	thread_count         int
	command              string // the userspace command responsible for launching this process
	is_kernel_thread     bool
	process_sched_prio   int
	process_sched_policy string
	uptime               uint64
}

func getInitialSnapshot() tuiModel {
	headerMsg := generateRandomTopHeader()
	deets := generateRandomDaemonMsg(5)
	progressBars := make([]progress.Model, 5)

	for i := range progressBars {
		progressBars[i] = progress.New(progress.WithDefaultGradient())
	}

	log.Println("Collecting init snapshot for display")
	log.Println("Collecting init snapshot for display")
	return tuiModel{
		header:   headerMsg,
		deets:    deets,
		progress: progressBars,
	}
}

func getMetrics() (topHeaderModel, []daemonMsg) {
	return generateRandomTopHeader(), generateRandomDaemonMsg(5)

}

func pollMetricsCmd() tea.Cmd {
	return tea.Tick(time.Second*1, func(t time.Time) tea.Msg {
		header, deets := getMetrics()
		return tuiModel{
			header: header,
			deets:  deets,
		}
	})
}

func initialModel() tuiModel {
	initSnapshot := getInitialSnapshot()
	return initSnapshot
}

func generateRandomTopHeader() topHeaderModel {
	rand.Seed(time.Now().UnixNano())

	res := topHeaderModel{}
	res.topCoreCpu = rand.Float64() * 100.0
	res.memory = float64(rand.Intn(101))
	res.swap = float64(rand.Intn(101))
	res.uptime = float64(rand.Intn(101))
	corescpu := make([]float64, 4)
	res.coresCpuUsage = corescpu
	for i := range 4 {
		res.coresCpuUsage[i] = rand.Float64() * 100.0
	}

	return res
}

func generateRandomDaemonMsg(n int) []daemonMsg {
	rand.Seed(time.Now().UnixNano())

	msg := make([]daemonMsg, n)

	processStates := []string{"Running", "Sleeping", "Stopped", "Zombie", "Idle"}
	commands := []string{"nginx", "sshd", "docker", "chrome", "python", "goapp"}
	schedPolicies := []string{"SCHED_OTHER", "SCHED_FIFO", "SCHED_RR", "SCHED_BATCH"}

	data := daemonMsg{}

	for i := 0; i < n; i++ {
		data.process_id = rand.Intn(5000) + 100 // PID 100 - 5100
		data.parent_pid = rand.Intn(1000) + 1   // PPID 1 - 1000
		data.process_name = fmt.Sprintf("proc_%d", i)
		data.process_state = processStates[rand.Intn(len(processStates))]
		data.vm_size = rand.Float64()*500000 + 10000 // 10 MB - 500 MB
		data.vm_res_mem = rand.Float64()*100000 + 1000
		data.cpuUsage = rand.Intn(101)        // 0-100%
		data.thread_count = rand.Intn(50) + 1 // 1 - 50 threads
		data.command = commands[rand.Intn(len(commands))]
		data.is_kernel_thread = rand.Intn(2) == 0
		data.process_sched_prio = rand.Intn(140) // priority 0 - 139
		data.process_sched_policy = schedPolicies[rand.Intn(len(schedPolicies))]
		data.uptime = uint64(rand.Intn(86400)) // up to 1 day in seconds

		msg[i] = data
	}

	return msg
}

func (m tuiModel) Init() tea.Cmd {
	tea.SetWindowTitle("Linux Process/Thread Monitor")
	return pollMetricsCmd()
}

func (m tuiModel) Update(msg tea.Msg) (tea.Model, tea.Cmd) {
	switch msg := msg.(type) {
	case tea.KeyMsg:
		if msg.String() == "q" || msg.String() == "ctrl+c" {
			return m, tea.Quit
		}

	case tuiModel:
		m.header = msg.header
		m.deets = msg.deets
		// updatedProgress, cmd2 := m.progress.Update(msg)
		// m.progress = updatedProgress.(progress.Model)
		// log.Printf("Set the porogress to %f\n", m.progress.Percent())
		return m, pollMetricsCmd()

	}
	return m, nil
}

func (m tuiModel) View() string {
	s := "\nA Linux Process/Thread Monitor with separate backend and orchestration\n"

	pad := strings.Repeat(" ", padding)
	s += fmt.Sprintf("\n%sCPU Total: %s", pad, m.progress[0].ViewAs(m.header.topCoreCpu/100.0))
	s += fmt.Sprintf("\n%sCore 0:    %s", pad, m.progress[1].ViewAs(m.header.coresCpuUsage[0]/100.0))
	s += fmt.Sprintf("\n%sCore 1:    %s", pad, m.progress[2].ViewAs(m.header.coresCpuUsage[1]/100.0))
	s += fmt.Sprintf("\n%sCore 2:    %s", pad, m.progress[3].ViewAs(m.header.coresCpuUsage[2]/100.0))
	s += fmt.Sprintf("\n%sCore 3:    %s\n", pad, m.progress[4].ViewAs(m.header.coresCpuUsage[3]/100.0))

	s += fmt.Sprintf("CPU Usage [%.2f]\n", m.header.topCoreCpu)
	s += fmt.Sprintf("Core 0 [%.2f]\n", m.header.coresCpuUsage[0])
	s += fmt.Sprintf("Core 1 [%.2f]\n", m.header.coresCpuUsage[1])
	s += fmt.Sprintf("Core 2 [%.2f]\n", m.header.coresCpuUsage[2])
	s += fmt.Sprintf("Core 3 [%.2f]\n", m.header.coresCpuUsage[3])
	s += fmt.Sprintf("Uptime : %.0f seconds\n", m.header.uptime)

	s += "ProcessID\tParentID\tProcessName\tProcessState\tCPU\tNThreads\n"

	displaySpace := "       "

	for i := range 5 {
		s += fmt.Sprintf("%d %s", m.deets[i].process_id, displaySpace)
		s += fmt.Sprintf("%d %s", m.deets[i].parent_pid, displaySpace)
		s += fmt.Sprintf("%s %s", m.deets[i].process_name, displaySpace)
		s += fmt.Sprintf("%s %s", m.deets[i].process_state, displaySpace)
		s += fmt.Sprintf("%d %s", m.deets[i].cpuUsage, displaySpace)
		s += fmt.Sprintf("%d %s\n", m.deets[i].thread_count, displaySpace)
	}

	return mainStyle.Render(s)
}

func main() {
	p := tea.NewProgram(initialModel(), tea.WithAltScreen())
	if _, err := p.Run(); err != nil {
		fmt.Printf("Alas, there's been an error\n")
		os.Exit(1)
	}
}

// func pollMetrics(ctx context.Context, interval time.Duration) tea.Cmd {
// 	return func() tea.Msg {
// 		t := time.NewTicker(interval)
// 		for {
// 			select {
// 			case <-t.C:
// 				snap := getMetrics()
// 				return snap
// 			case <-ctx.Done():
// 				return nil
// 			}
// 		}
// 	}
// }
