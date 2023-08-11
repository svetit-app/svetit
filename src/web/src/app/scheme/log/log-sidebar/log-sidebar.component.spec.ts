import { ComponentFixture, TestBed } from '@angular/core/testing';

import { LogSidebarComponent } from './log-sidebar.component';

describe('SidebarComponent', () => {
  let component: LogSidebarComponent;
  let fixture: ComponentFixture<LogSidebarComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ LogSidebarComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(LogSidebarComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
