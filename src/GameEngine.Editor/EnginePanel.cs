using System.Windows.Forms;

namespace GameEngine.Editor
{
    public class EnginePanel : Panel
    {
        public EnginePanel()
        {
            SetStyle(ControlStyles.Opaque | ControlStyles.UserPaint | ControlStyles.AllPaintingInWmPaint, true);
            UpdateStyles();
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            // Don't call base.OnPaint - engine handles rendering
        }

        protected override void OnPaintBackground(PaintEventArgs e)
        {
            // Don't paint background - engine handles it
        }
    }
}
